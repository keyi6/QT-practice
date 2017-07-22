let url = require('url'),
    http = require('http'),
    util = require('util'),
    mysql = require('mysql'),
    dgram = require('dgram'),
    crypto = require('crypto'),
    querystring = require('querystring')

let port = 8321,
    md5 = function(str) { // {{{
      return crypto.createHash("md5WithRSAEncryption").update(str).digest("hex")
    }, // }}}
    connection = mysql.createConnection({ // {{{
      host: "localhost",
      user: "user",
      password: "password",
      database: "IM"
    }), // }}}
    broadcastToAll = function(message, logMsg) {
      message = new Buffer(message)
      let socket = dgram.createSocket("udp4")
      socket.bind(function () { socket.setBroadcast(true); });
        
      connection.query('select ip from user', function(__err__, __rows__) {
        console.log(__rows__)
        for (let i = 1; i < __rows__.length; i ++)
            if (__rows__[i].ip != undefined && /192\.168\./.test(__rows__[i].ip))
              socket.send(message, 0, message.length, 8323, __rows__[i].ip, function(err, bytes) {} ) 
      })
      console.log(logMsg)
    }
            
connection.connect()

let server = dgram.createSocket('udp4')

// send and receive message // {{{
server.on("message", function (msg, rinfo) { 
  msg = JSON.parse(msg)
  if (msg.msg != undefined) {
    connection.query("insert into record (sender, receiver, msg) values (" + msg.sender + ", " + msg.receiver + ", \"" + msg.msg + "\");", function (err, rows) { })
    connection.query('select date_format(recTime,"%Y %b %d %h:%i:%s %p") from record where id=last_insert_id()', function (err, rows) {
      if (!(err || rows[0] == undefined)) {
        msg.time = rows[0]['date_format(recTime,"%Y %b %d %h:%i:%s %p")']
        msg.type = "message"
        msg.sender = eval(msg.sender)
        msg.receiver = eval(msg.receiver)
        console.log("server got: " + msg + " from " + rinfo.address + ":" + rinfo.port);
      
        connection.query('SELECT * from user WHERE id=' + msg.receiver, function(err, _rows) {
          if (!(err || _rows[0] == undefined)) {
            let _data_ = new Buffer(JSON.stringify(msg))
            let socket = dgram.createSocket("udp4")
            socket.bind(function () { socket.setBroadcast(true); });
            socket.send(_data_, 0, _data_.length, port + 2, rinfo.address, function(err, bytes) { 
              socket.send(_data_, 0, _data_.length, port + 2, _rows[0].ip , function(err, bytes) { 
                socket.close() 
                console.log('[TRANSFER SUCCESS]' + _data_) }) }) } }) } })
  } else if (msg.groupMsg != undefined) {
    console.log('[GROUPMSG] received ' + JSON.stringify(msg))
    
    let ret = msg;
    ret.type = "groupMsg"
    ret.senderId = eval(msg.sender)
    connection.query('select username from user where id=' + ret.senderId, function(err, rows) { 
      ret.senderUsername = rows[0].username 
      ret = JSON.stringify(ret)
      broadcastToAll(ret, '[GROUPMSG] transfer successfully')
    })
  }
}) 
server.on("listening", function () { console.log("listening infomation from port: " + (port + 1).toString() + "\n") })
server.bind(port + 1) // }}}

// server
http.createServer(function (req, res) {
  let postBody = ''
  res.writeHeader(200, {
    "Content-Type": "text/plain",
    "Access-Control-Allow-Origin": "*"
  })

  req.on('data', function(chunk) { postBody += chunk })

  req.on('end', function() {
    console.log('\n')
    let getInfo = url.parse(req.url)
    let path = getInfo.pathname

    if (path == '/getUserInfo') { // {{{
      let info = querystring.parse(getInfo.query)
      if (getInfo.pathname == '/getUserInfo') {
        if (info.id) {
          connection.query('SELECT * from user WHERE id=' + info.id, function(err, rows) { 
            if (err || rows[0] == undefined) {
              res.end('')
            } else {
              delete rows[0]['password']
              res.end(JSON.stringify(rows[0]))
              console.log('[GETUSERINFO]' + JSON.stringify(rows[0]))
            }
          })
        } else if (info.ip) {
          connection.query('SELECT * from user WHERE ip="' + info.ip + '"', function(err, rows) { 
            if (err || rows[0] == undefined) {
              res.end('');
            } else {
              delete rows[0]['password']
              res.end(JSON.stringify(rows[0]))
              console.log('[GETUSERINFO]' + JSON.stringify(rows[0]))
            }
          })
        }
      } //}}}

    } else if (path == '/getHistory') { // {{{
      let info = querystring.parse(getInfo.query)
      connection.query('select sender,msg,DATE_FORMAT(recTime,"%Y-%m-%d %H:%i:%s") from record where (sender=' + info.p1 + ' and receiver=' + info.p2 + ') or (sender=' + info.p2 + ' and receiver=' + info.p1 + ')', function(err, rows) {
        if (!(err || rows == undefined)) {
          let ret = JSON.stringify(rows)
          console.log('[HISTORY]' + ret)
          res.end(ret)
        }
      }) // }}}
    
    } else if (path == '/logout') { // {{{
      let info = querystring.parse(getInfo.query)
      info.id = eval(info.id)
      connection.query('update user set ip="" where id=' + info.id, function(err, _rows_) {
        res.end('{ "code": 1 }')
        info.type = "logout"
        info = JSON.stringify(info)
        console.log("[LOGOUT] Info received." + info)

        broadcastToAll(info, '[LOGOUT] success!')
      }) // }}}

    } else if (path == '/rlogin') { // {{{
      console.log('[RLOGIN]')
      let info = querystring.parse(getInfo.query)
      console.log(info)
      connection.query('SELECT * from user WHERE id=' + info.id, function(err, rows) { 
        if (err || rows[0] == undefined) {
          console.log('[RLOGIN ERROR 1] No such user')
        } else {
          let ret = rows[0]
          delete ret['password']
          ret.type = 'rlogin'

          let message = new Buffer(JSON.stringify(ret)),
              socket = dgram.createSocket("udp4")
          socket.bind(function () { socket.setBroadcast(true); });
          socket.send(message, 0, message.length, 8323, info.ip , function(err, bytes) { 
            console.log(ret.ip)
            if (err) {
              console.log('[RLOGIN ERROR 2] transfer error')
            } else {
              socket.close()
              res.end(' {"code" : "1"} ')
            }
          })
        }
      }) // }}}
      
    } else if (path == "/login") { // {{{
      let info = querystring.parse(postBody)
      console.log("[LOGIN] Info received.")

      //check
      connection.query('SELECT * from user WHERE username = "' + info.username + '"', function(err, rows) {
        if (err) {  
          res.end('{ "type": "login", "code":"-3" }')
        } else if (rows[0] == undefined) {
          console.log("[LOGIN ERROR] No user named " + info.username)  
          res.end('{ "type": "login", "code":"-1" }')
        } else if (rows[0].password != md5(info.password)) {
          res.end('{ "type": "login", "code":"-2" }')
          console.log("[LOGIN ERROR] wrong password for user " + info.username)  
        } else {
          connection.query('update user set ip=\'' + info.ip + '\' where id=' + rows[0].id, function(err, _rows) {
            delete rows[0]['password']
            let ret = rows[0]
            ret.type = 'login'
            ret.ip = info.ip
            ret.code = '1'
            ret = JSON.stringify(ret)
            
            broadcastToAll(ret, '[LOGIN] ' + rows[0].username + ' successfully login')
            res.end(ret)
          })
        }
      })// }}}

    } else if (path == "/register") { // {{{
      let info = querystring.parse(postBody)
      console.log("[REGISTER] Info received.")
      console.log(info)
      
      connection.query('SELECT password from user WHERE username = "' + info.username + '"', function(err, rows) {
        if (err) {  
          res.end('{ "type": "register", "code": "-4" }')
        } else if (rows[0] == undefined) {
          connection.query('insert into user (username, password) values (\'' + info.username + '\', \'' + md5(info.password) + '\')', function(err, rows) {
            if (err) {
              res.end('{ "type": "register", "code": "-4" }')
            } else {
              res.end('{ "type": "register", "code": "1" }')
              console.log("[REGISTER] new user " + info.username)
            }
          })
        } else {
          res.end('{ "type": "register", "code": "-2" }')
        }
      })
    } //}}}
  })
}).listen(port)

console.log("\033cserver is running at localhost:" + port)
