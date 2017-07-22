#include "index.h"
#include "ui_index.h"

ChatWidget *chatWidget;

Index::Index(QWidget *parent, QJsonObject _user) :
    QWidget(parent),
    user(_user),
    ui(new Ui::Index) {

    ui->setupUi(this);

    QFont font("Microsoft Yahei");
    setFont(font);
    QIcon icon(":/new/prefix1/assets/im.png");
    setWindowIcon(icon);
    setFixedSize(800,600);
    setWindowTitle("IM v1.0 © chili");

    /***  Network Service Setup  ***/
    nr = new NetworkRequest;
    nr -> UdpInit(user["id"].toInt());
    connect(nr, SIGNAL(NewFriend()), this, SLOT(newFriend()));
    connect(nr, SIGNAL(NewMessage()), this, SLOT(newMessage()));
    connect(nr, SIGNAL(rNewFriend()), this, SLOT(rnewFriend()));
    connect(nr, SIGNAL(FriendLogout(int)), this, SLOT(friendLogout(int)));
    connect(nr, SIGNAL(YouAreLoggedOutBecauseYouLoggedInAnotherDevice()), this, SLOT(youAreLoggedOutBecauseYouLoggedInAnotherDevice()));
    connect(nr,SIGNAL(NewGroupMessage(int,QString,QString)),this,SLOT(appendGroupNewMessage(int,QString,QString)));

    /***  Friend List  ***/
    QScrollArea * scrollFriendList = new QScrollArea(this);
    fl = new FriendList(scrollFriendList);
    scrollFriendList -> setGeometry(0, 140, 280, 460);
    scrollFriendList -> setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollFriendList -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollFriendList -> setWidget(fl);
    connect(fl,SIGNAL(friendIdSignal(int)),this,SLOT(createChatWidget(int)));
    connect(fl,SIGNAL(showOneChatWidget(int)),this,SLOT(showChatWidget(int)));
    connect(fl, SIGNAL(friendIdSignalWhenReceivingNewMessage(int, int, QString)), this, SLOT(appendNewMessage(int, int, QString)));
    connect(fl, SIGNAL(showGroupWidget()), this, SLOT(showGroupChatWidget()));

    /***  GroupChat Widget ***/
    int i = user["id"].toInt();
    groupChatWidget = new GrounpChatWidget(this,i);
    groupChatWidget -> setGeometry(260, -30, 540, 660);
    groupChatWidget -> setFocusPolicy(Qt::NoFocus);

    /***  Personal Info  ***/
    userName = user["username"].toString();
    userIp = user["ip"].toString();

    userInformation = new QLabel(userName,this);
    userInformation -> setGeometry(0,0,260,140);
    userInformation -> setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    userInformation -> setFocusPolicy(Qt::NoFocus);
    userInformation -> setStyleSheet("background-color:rgba(252,98,77,180);color:#fff;border: 0px;");
    userInformation -> setText(user["username"].toString() + "\n" + user["ip"].toString()+"\n");

    userHeadPortrait = new QLabel(this);
    userHeadPortrait -> setPixmap(QPixmap(":/new/prefix1/assets/user.png"));
    userHeadPortrait -> setGeometry(105, 23, 60, 60);
    userHeadPortrait -> setStyleSheet("border: 0px;");
}

Index::~Index() {
    delete ui;
}

void Index::newFriend() {
    QJsonObject temp_f = nr -> getNewFriend();
    fl -> addNewFriendToList(temp_f);

    JSON data;
    data["id"] = QString::number(user["id"].toInt());
    data["ip"] = temp_f["ip"].toString();
    nr -> get(data, "rlogin");
}

void Index::newMessage() {
    QJsonObject temp_m = nr -> getNewMessage();
    fl -> getNewMessage(temp_m);
}

void Index::rnewFriend() {
    fl -> addNewFriendToList(nr -> getNewFriend());
}

void Index::createChatWidget(int i){
    int friendId = i;
    ChatWidget *chatWidget = new ChatWidget(this, friendId, user["id"].toInt());
    chatWidget -> setGeometry(260, -30, 540, 660);
    chatWidget -> setFocusPolicy(Qt::NoFocus);
    chatWidget -> hide();
    vectorChatWidget.append(chatWidget);

}

void Index::showChatWidget(int exp){
    vectorChatWidget[exp] -> show();

    for (int i = 0; i < vectorChatWidget.size(); i ++)
        if (i != exp)
            vectorChatWidget[i] -> hide();

    groupChatWidget -> hide();
}

void Index::appendNewMessage(int i, int sender, QString msg) {
    vectorChatWidget[i] -> appendNewMessage(sender, msg);
}
void Index::appendGroupNewMessage(int sender, QString senderUsername, QString msg){
    qDebug()<<sender<<senderUsername<<msg;
    groupChatWidget -> appendNewMessage(sender,senderUsername,msg);
}

void Index::youAreLoggedOutBecauseYouLoggedInAnotherDevice() {
    WarningDialog * wd = new WarningDialog;
    wd -> exec();
    this -> close();
}

void Index::closeEvent(QCloseEvent * e) {
    JSON logoutData;
    logoutData["id"] = QString::number(user["id"].toInt());

    nr -> get(logoutData, "logout");
}

void Index::friendLogout(int fid) {
    fl -> removeFriendFromList(fid);
}

void Index::showGroupChatWidget() {
    groupChatWidget -> show();

    for (int i = 0; i < vectorChatWidget.size(); i ++)
        vectorChatWidget[i] -> hide();
}
