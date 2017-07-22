#include "networkrequest.h"


// OurEDA
//QString NetworkRequest::hostUrl = "http://192.168.74.175:8321/";
//QString NetworkRequest::hostGateWay = "192.168.74.175";

/* DLUT
QString NetworkRequest::hostUrl = "http://192.168.47.192:8321/";
QString NetworkRequest::hostGateWay = "192.168.47.192";
*/
QString NetworkRequest::hostUrl = "http://192.168.0.104:8321/";
QString NetworkRequest::hostGateWay = "192.168.0.104";

NetworkRequest::NetworkRequest(QObject * _parent) : QObject(_parent){
}

// format convert
QString NetworkRequest::JSONtoQuerystring(JSON data) {
    QString ret = "";
    for (JSON::iterator it = data.begin(); it != data.end(); it ++)
        ret += it.key() + "=" + it.value() + "&";

    if (ret.length())
        ret = ret.mid(0, ret.length() - 1);

    return ret;
}

QString NetworkRequest::JSONtoJSONString(JSON data) {
    QString ret = "{ ";
    for (JSON::iterator it = data.begin(); it != data.end(); it ++)
        ret += "\"" + it.key() + "\": \"" + it.value() + "\",";

    ret = ret.mid(0, ret.length() - 1);
    ret += " }";
    return ret;
}

QJsonObject NetworkRequest::ByteArrayToJsonObject(QByteArray str) {
    QString temp = str.data();
    temp = temp.toUtf8();
    str.clear();
    str.append(temp);

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(str, &json_error);

    if (json_error.error == QJsonParseError::NoError && parse_doucment.isObject())
        return parse_doucment.object();

    QJsonObject ept;
    return ept;
}

QJsonObject NetworkRequest::post(JSON data, QString action) {
    QNetworkRequest * req = new QNetworkRequest;
    req -> setUrl(QUrl(hostUrl + action));
    req -> setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QByteArray postData;
    postData.append(JSONtoQuerystring(data));

    QNetworkAccessManager * net = new QNetworkAccessManager;
    QNetworkReply * reply = net -> post(* req, postData);
    QEventLoop temp_loop;
    connect(reply, SIGNAL(finished()), &temp_loop, SLOT(quit()));

    QTimer * timer = new QTimer;
    timer -> setInterval(10000);
    connect(timer, SIGNAL(timeout()), & temp_loop, SLOT(quit()));
    timer -> start();

    temp_loop.exec();
    if (reply -> error() == QNetworkReply::NoError) {
        QByteArray bytes = reply -> readAll();
        reply -> deleteLater();
        return ByteArrayToJsonObject(bytes);
    }
}

QJsonObject NetworkRequest::get(JSON data, QString action) {
    QNetworkRequest * req = new QNetworkRequest;
    req -> setUrl(QUrl(hostUrl + action + "?" + JSONtoQuerystring(data)));

    QNetworkAccessManager * net = new QNetworkAccessManager;
    QNetworkReply * reply = net -> get(* req);
    QEventLoop temp_loop;
    connect(reply, SIGNAL(finished()), &temp_loop, SLOT(quit()));

    QTimer * timer = new QTimer;
    timer -> setInterval(10000);
    connect(timer, SIGNAL(timeout()), & temp_loop, SLOT(quit()));
    timer -> start();

    temp_loop.exec();
    if (reply -> error() == QNetworkReply::NoError) {
        QByteArray bytes = reply -> readAll();
        qDebug() << bytes;

        reply -> deleteLater();
        return ByteArrayToJsonObject(bytes);
    }
}

QVector<QJsonObject> NetworkRequest::getHistory(int p1, int p2) {
    QNetworkRequest * req = new QNetworkRequest;
    req -> setUrl(QUrl(hostUrl + "getHistory?p1=" + QString::number(p1) +
                  "&p2=" + QString::number(p2)));

    QNetworkAccessManager * net = new QNetworkAccessManager;
    QNetworkReply * reply = net -> get(* req);
    QEventLoop temp_loop;
    connect(reply, SIGNAL(finished()), &temp_loop, SLOT(quit()));

    QVector<QJsonObject> ret;

    QTimer * timer = new QTimer;
    timer -> setInterval(10000);
    connect(timer, SIGNAL(timeout()), & temp_loop, SLOT(quit()));
    timer -> start();

    temp_loop.exec();
    if (reply -> error() == QNetworkReply::NoError) {
        QByteArray bytes = reply -> readAll();
        QString tempString = bytes.data();
        QStringList tempStringList = tempString.split("},{");

        qDebug() << tempStringList.size();
        for(int i = 0; i < tempStringList.size(); i ++) {
            tempString = tempStringList[i];
            if (i == 0)
                tempString.replace("[{", "");
            if (i == tempStringList.size() - 1)
                tempString.replace("}]", "");

            tempString = "{" + tempString + "}";

            QByteArray tempByte;
            tempByte.append(tempString);
            ret.push_back(ByteArrayToJsonObject(tempByte));
        }
        reply -> deleteLater();
        return ret;
    }
}

QJsonObject NetworkRequest::getUserInfoById(int id) {
    JSON data;
    data["id"] = QString::number(id);
    return get(data, "getUserInfo");
}

QString NetworkRequest::getLocalIp() {
    QList<QHostAddress>listAddress = QNetworkInterface::allAddresses();
    for (int j = 0; j < listAddress.size(); j ++)
        if (! listAddress.at(j).isNull() && listAddress.at(j).toString().mid(0, 8) == "192.168.")
            return listAddress.at(j).toString();
}

void NetworkRequest::UdpInit(int _myself) {
    myself = _myself;
    UdpReceiver = new QUdpSocket;
    UdpReceiver -> bind(8323, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(UdpReceiver, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void NetworkRequest::readPendingDatagrams() {
    while (UdpReceiver -> hasPendingDatagrams()) {
        QByteArray datagram;

        datagram.resize(UdpReceiver -> pendingDatagramSize());
        UdpReceiver -> readDatagram(datagram.data(), datagram.size());
        QJsonObject json = ByteArrayToJsonObject(datagram.data());

        qDebug() << "[RECEIVED]" << datagram << json;

        if (json["type"].toString() == "login") {
            if (json["id"].toInt() != myself) {
                recv_fd = json;
                emit NewFriend();
            }
            else
                emit YouAreLoggedOutBecauseYouLoggedInAnotherDevice();

        } else if (json["type"].toString() == "message") {
            recv_msg = json;
            emit NewMessage();
        } else if (json["type"].toString() == "rlogin") {
            recv_fd = json;
            emit rNewFriend();
        } else if (json["type"].toString() == "logout") {
           emit FriendLogout(json["id"].toInt());
        } else if (json["type"].toString() == "groupMsg") {
            emit NewGroupMessage(json["senderId"].toInt(),  json["senderUsername"].toString(), json["groupMsg"].toString());
        }
    }
}

QJsonObject NetworkRequest::getNewFriend() {
    return recv_fd;
}

QJsonObject NetworkRequest::getNewMessage() {
    return recv_msg;
}
