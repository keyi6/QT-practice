#include "sever.h"
#include "ui_sever.h"

Sever::Sever(QWidget *parent,int _friendId) :
    QDialog(parent),
    ui(new Ui::Sever)
{
    ui->setupUi(this);
    senderId = _friendId;
    setWindowTitle("receive file");
    isAccepted = false;
    fileNameSize = 0;
    totalBytesServer = 0;
    bytesReceived = 0;
    nr = new NetworkRequest;
    senderName = new QLabel(this);
    QString sendername = nr->getUserInfoById(senderId)["username"].toString();
    qDebug()<<"[SERVER]"<<sendername;
    senderName -> setText(sendername);
    senderName -> setGeometry(30,30,100,30);
    senderName -> setStyleSheet("background-color:#fff;color:#333;border:1px solid grey;");

    showReceiveFileName = new QLabel(this);
    showReceiveFileName -> setText("send you a file");
    showReceiveFileName -> setGeometry(30,90,350,30);
    showReceiveFileName -> setStyleSheet("background-color:#fff;color:#333;border:1px solid grey;");

    showCurServerStatus = new QLabel(this);
    showCurServerStatus -> setGeometry(30,60,300,30);
    showCurServerStatus -> setStyleSheet("color:#333;border:0px;");
    showCurServerStatus -> setText("Do you want to accept?");

    serverProgressBar = new QProgressBar(this);
    serverProgressBar -> setGeometry(50,150,300,30);
    serverProgressBar -> setRange(0,100);
    serverProgressBar -> setValue(0);
    serverProgressBar -> setStyleSheet("QProgressBar::chunk {background-color: rgba(252,98,77,180);border-radius: 5px;}QProgressBar {background-color:#fff;border:0px; border-radius: 5px;   text-align: center;}");

    tcpServerStart();
    connect(&tcpServer,SIGNAL(newConnection()),this,SLOT(showServerDialog()));
}

void Sever::tcpServerStart(){
    QString myAdd = nr -> getLocalIp();
    if(!tcpServer.listen(QHostAddress(myAdd),8453)){
        qDebug()<<"listen failed";
        return;
    }

    qDebug() << "listen LocalHost 8453";
}
void Sever::showServerDialog(){
    show();
        showCurServerStatus->setText("accepted connection");
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection,SIGNAL(readyRead()),this,SLOT(updateServerProgress()));
    connect(tcpServerConnection,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

}

void Sever::updateServerProgress()  //更新进度条，接收数据
{
   // qDebug()<<"--------updateServerProgress-----------";
   QDataStream in(tcpServerConnection);
   in.setVersion(QDataStream::Qt_5_9);
   if(bytesReceived <= sizeof(qint64)*2)
   { //如果接收到的数据小于16个字节->刚开始接收数据，保存头文件信息
       if((tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize == 0))
       { //接收数据总大小信息和文件名大小信息
           in >> totalBytesServer >> fileNameSize;
           bytesReceived += sizeof(qint64) * 2;
       }
       if((tcpServerConnection->bytesAvailable() >= fileNameSize) && (fileNameSize != 0))
       {  //接收文件名，并建立文件
           in >> receiveFileName;
           showCurServerStatus->setText("accepting file");
           bytesReceived += fileNameSize;
           localFileReceive= new QFile(receiveFileName);
           if(!localFileReceive->open(QFile::WriteOnly))
           {
                qDebug() << "open file error!";
                return;
           }
       }
       else return;
   }
   if(bytesReceived < totalBytesServer)
   {  //如果接收的数据小于总数据，那么写入文件
      bytesReceived += tcpServerConnection->bytesAvailable();
      inBlock= tcpServerConnection->readAll();
      localFileReceive->write(inBlock);
      inBlock.resize(0);
   }
 //更新进度条
   serverProgressBar->setMaximum(totalBytesServer);
   serverProgressBar->setValue(bytesReceived);

   if(bytesReceived == totalBytesServer)
   { //接收完成时
    tcpServerConnection->close();
    localFileReceive->close();
    showCurServerStatus->setText("accepted");
   }
}
void Sever::displayError(QAbstractSocket::SocketError) //错误处理
{   qDebug()<<"--------displayError-----------";
    qDebug() << tcpServerConnection->errorString();
    tcpServerConnection->close();
    serverProgressBar->reset();
    showCurServerStatus->setText("Server ready");
}

Sever::~Sever()
{
    delete ui;
}
