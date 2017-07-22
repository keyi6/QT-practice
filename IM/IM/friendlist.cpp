#include "friendlist.h"
#include "ui_friendlist.h"

QString FriendList::avatarBackgroundColor[] = {"#18587A", "#FC624D", "#00AAA0", "#FAF15D", "#0962EA", "#F6318C"};

FriendList::FriendList(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FriendList)
{
    ui->setupUi(this);

    setFixedWidth(260);
    setMinimumHeight(460);
    setAutoFillBackground(true);
    setPalette(QColor("#F3F3F3"));

    QLabel * tempInfo = new QLabel(this);
    tempInfo -> setStyleSheet("background-color:#F3F3F3; color: #000; border: 0px;");
    tempInfo -> setText("<h3>Group Chat</h3>");
    tempInfo -> setGeometry(80, 0, 260, fixedHeight);

    QLabel * tempLabel = new QLabel(this);
    tempLabel -> setFixedSize(48, 48);
    tempLabel -> setStyleSheet("border: 0px; border-radius: 24px;");
    tempLabel -> setPixmap( QPixmap(":/new/prefix1/assets/group2.png"));
    tempLabel -> setGeometry(20, 16, 48, 48);

    QLabel * tempMessageUnread = new QLabel(this);
    tempMessageUnread -> setFixedSize(20, 20);
    tempMessageUnread -> setAlignment(Qt::AlignHCenter);
    tempMessageUnread -> setGeometry(230, 30, 20, 20);
    tempMessageUnread -> show();

    gBtn = new QPushButton(this);
    gBtn -> setFixedSize(260, fixedHeight);
    gBtn -> setGeometry(0, 0, 260, fixedHeight);
    gBtn -> setFlat(true);

    connect(gBtn, SIGNAL(clicked()), this, SIGNAL(showGroupWidget()));
}

FriendList::~FriendList()
{
    delete ui;
}

void FriendList::addNewFriendToList(QJsonObject newFriend) {
    int cnt = friendList.size() + 1;
    for (int i = 0; i < cnt - 1; i ++)
        if (friendList[i]["id"].toInt() == newFriend["id"].toInt()) {
            friendList[i] = newFriend;
            return;
        }

    QLabel * tempInfo = new QLabel(this);
    tempInfo -> setStyleSheet("background-color:#F3F3F3; color: #000; border: 0px;");
    tempInfo -> setText("<h3>" + newFriend["username"].toString() + "</h3>" +
                "<p>" + newFriend["ip"].toString() + "</p>");
    tempInfo -> setGeometry(80, cnt * fixedHeight, 260, fixedHeight);
    tempInfo -> show();
    friendListInfo.push_back(tempInfo);

    QLabel * tempLabel = new QLabel(this);
    tempLabel -> setFixedSize(48, 48);
    tempLabel -> setStyleSheet("border: 0px; border-radius: 24px; background-color: " + avatarBackgroundColor[friendListAvator.size() % 6]);
    tempLabel -> setPixmap( QPixmap(":/new/prefix1/assets/user.png"));
    tempLabel -> setGeometry(20, 16 + cnt * fixedHeight, 48, 48);
    tempLabel -> show();
    friendListAvator.push_back(tempLabel);

    QLabel * tempMessageUnread = new QLabel(this);
    tempMessageUnread -> setFixedSize(20, 20);
    tempMessageUnread -> setAlignment(Qt::AlignHCenter);
    tempMessageUnread -> setGeometry(230, 30 + cnt * fixedHeight, 20, 20);
    tempMessageUnread -> show();
    messageUnread.push_back(tempMessageUnread);

    QPushButton * tempBtn = new QPushButton(this);
    tempBtn -> setFixedSize(260, fixedHeight);
    tempBtn -> setGeometry(0, cnt * fixedHeight, 260, fixedHeight);
    tempBtn -> setFlat(true);
    tempBtn -> show();
    friendButton.push_back(tempBtn);
    friendId = newFriend["id"].toInt();
    emit friendIdSignal(friendId);
    connect(tempBtn, SIGNAL(clicked()), this, SLOT(showChatWidget()));

    unread.push_back(0);
    friendList.push_back(newFriend);
    resize(260, (cnt + 1) * fixedHeight);
}

void FriendList::showChatWidget() {
    QPushButton * pressedKey = qobject_cast<QPushButton *>(sender());
    int mark_i;
    for (int i = 0; i < friendButton.size(); i ++)
        if (pressedKey == friendButton[i]) {
            mark_i = i;
            unread[i] = 0;
            messageUnread[i] -> setStyleSheet("border: 0px; border-radius: 10px; background-color: #F3F3F3;");
            messageUnread[i] -> setText("");
            emit showOneChatWidget(mark_i);
            break;
        }
}

void FriendList::getNewMessage(QJsonObject sender) {
    int mark_i, ssid = sender["sender"].toInt(), sfid = sender["receiver"].toInt();
    for (int i = 0, fid; i < friendList.size(); i ++) {
        fid = friendList[i]["id"].toInt();
        if (ssid == fid || sfid == fid) {
            if (ssid == fid) {
                messageUnread[i] -> setStyleSheet("border: 0px; border-radius: 10px; background-color: red; color: #fff; font-size: 16px;");
                messageUnread[i] -> setText(QString::number(++ unread[i]));
            }

            mark_i = i;
            emit friendIdSignalWhenReceivingNewMessage(mark_i, sender["sender"].toInt(), sender["msg"].toString());
            break;
        }
    }
}

void FriendList::removeFriendFromList(int fid) {
    int mark_i = -1;
    for (int i = 0; i < friendList.size(); i ++)
        if (friendList[i]["id"].toInt() == fid) {
            mark_i = i;
            break;
        }

    if (~ mark_i) {
        for (int i = mark_i, j; i < friendList.size(); i ++) {
            j = i - 1;
            friendButton[i] -> setGeometry(0, j * fixedHeight, 260, fixedHeight);
            messageUnread[i] -> setGeometry(230, 30 + j * fixedHeight, 20, 20);
            friendListInfo[i] -> setGeometry(80, j * fixedHeight, 260, fixedHeight);
            friendListAvator[i] -> setGeometry(20, 16 + j * fixedHeight, 48, 48);
        }
        friendButton[mark_i] -> hide();
        messageUnread[mark_i] -> hide();
        friendListInfo[mark_i] -> hide();
        friendListAvator[mark_i] -> hide();

        unread.erase(unread.begin() + mark_i);
        friendList.erase(friendList.begin() + mark_i);
        friendButton.erase(friendButton.begin() + mark_i);
        messageUnread.erase(messageUnread.begin() + mark_i);
        friendListInfo.erase(friendListInfo.begin() + mark_i);
        friendListAvator.erase(friendListAvator.begin() + mark_i);
    }

}
