#ifndef NETWORKREQUEST_H

#define NETWORKREQUEST_H

#include <QDebug>

#include <QMap>
#include <QList>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QVector>
#include <QObject>
#include <QByteArray>
#include <QEventLoop>
#include <QJsonArray>
#include <QUdpSocket>
#include <QJsonObject>
#include <QHostAddress>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkInterface>
#include <QNetworkAccessManager>

#define JSON QMap<QString, QString>

class NetworkRequest : public QObject
{
    Q_OBJECT
public:
    explicit NetworkRequest(QObject * _parent = nullptr);

    QJsonObject post(JSON data, QString action);
    QJsonObject get(JSON data, QString action);

    QJsonObject getUserInfoById(int id);
    QVector<QJsonObject> getHistory(int p1, int p2);


    // 如果要接受信息，请先调用 UdpInit()
    void UdpInit(int _myself);

    QString getLocalIp();
    QJsonObject getNewFriend();
    QJsonObject getNewMessage();
    QString JSONtoJSONString(JSON data);


    static QString hostUrl, hostGateWay;

private slots:
    void readPendingDatagrams();

signals:
    void NewFriend();
    void NewMessage();
    void rNewFriend();
    void FriendLogout(int id);
    void NewGroupMessage(int senderId, QString senderUsername, QString msg);
    void YouAreLoggedOutBecauseYouLoggedInAnotherDevice();

private:
    int myself;
    QJsonObject recv_msg, recv_fd;
    QUdpSocket * UdpReceiver;

    QJsonObject ByteArrayToJsonObject(QByteArray str);
    QString JSONtoQuerystring(JSON data);
};

#endif // NETWORKREQUEST_H
