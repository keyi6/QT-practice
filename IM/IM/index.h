#ifndef INDEX_H
#define INDEX_H

#include <QMap>
#include <QString>
#include <QWidget>
#include <QVector>
#include <QTextCodec>
#include <QUdpSocket>
#include <QJsonArray>
#include <QCloseEvent>
#include <QScrollArea>
#include <QJsonObject>
#include <QJsonDocument>

#include "friendlist.h"
#include "chatwidget.h"
#include "grounpchatwidget.h"
#include "warningdialog.h"
#include "networkrequest.h"

#include <QDebug>

#define JSON QMap<QString, QString>

namespace Ui {
class Index;
}

class Index : public QWidget
{
    Q_OBJECT

public:
    explicit Index(QWidget *parent = 0, QJsonObject _user = QJsonObject());
    ~Index();

private slots:
    void newFriend();
    void rnewFriend();
    void newMessage();

public slots:
    void appendNewMessage(int i, int sender, QString msg);
    void createChatWidget(int i);
    void showChatWidget(int i);
    void youAreLoggedOutBecauseYouLoggedInAnotherDevice();
    void appendGroupNewMessage(int sender, QString senderUsername, QString msg);
    void friendLogout(int fid);
    void showGroupChatWidget();

private:
    Ui::Index *ui;

    QJsonObject user;
    QString userName, userIp;
    QByteArray broadCastInfo;

    QLabel *userInformation;
    QLabel *userHeadPortrait;
    GrounpChatWidget *groupChatWidget;

    FriendList * fl;
    NetworkRequest * nr;
    QVector<ChatWidget *>vectorChatWidget;

protected:
    void closeEvent(QCloseEvent * e);
};

#endif // INDEX_H
