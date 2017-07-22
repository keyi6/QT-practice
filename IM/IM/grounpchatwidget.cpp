#include "grounpchatwidget.h"
#include "ui_grounpchatwidget.h"

GrounpChatWidget::GrounpChatWidget(QWidget *parent,int _sender) :
    QWidget(parent),
    sender(_sender),
    ui(new Ui::GrounpChatWidget)
{

    ui->setupUi(this);
    qDebug()<<"group chat widget sender"<<sender;
    QFont font("Microsoft Yahei");
    setFont(font);
    setStyleSheet("background-color: #fff;border: 0px;");

    offsetHeight = 30 + 10;

    chatScrollArea = new QScrollArea(this);
    chatScrollArea -> setGeometry(0, 30,540,417);
    chatScrollArea -> setStyleSheet("background-color:rgba(245,245,245,250);border: 0px;");

    chatWidgetName = new QLabel(chatScrollArea);
    chatWidgetName -> setGeometry(0, 0, 540, 30);
    chatWidgetName -> setFocusPolicy(Qt::NoFocus);
    chatWidgetName -> setStyleSheet("background-color:rgba(245,245,245,250);border-bottom: 1px solid rgba(235,235,235,250);");
  //  QJsonObject myFriend = nr -> getUserInfoById(_receiver);
    chatWidgetName -> setText("Group Chat");

    chatScreen = new QLabel(this);
    chatScreen -> setFixedWidth(523);
    chatScreen -> setGeometry(0, 30, 523, 10);
    chatScreen -> setFocusPolicy(Qt::NoFocus);
    chatScreen -> adjustSize();
    chatScrollArea -> setWidget(chatScreen);

    iw = new GroupInputWidget(this, sender);
    iw -> setGeometry(0, 445, 540, 183);
}
int GrounpChatWidget::getSenderId(){
    return sender;
}
QString GrounpChatWidget::formatMessage(QString msg) {
    int cutLen = 48;
    QString ret = "";
    for (int i = 0; i < msg.length(); i += cutLen)
        ret += msg.mid(i, cutLen) + "\n";


    ret = ret.mid(0, ret.length() - 1);
    return ret;
}

void GrounpChatWidget::appendNewMessage(int senderId,QString _senderName, QString msg) {
    qDebug()<<sender<<_senderName<<msg;
    QLabel * temp = new QLabel(chatScreen);
    temp -> setMaximumWidth(400);
    temp -> setMinimumHeight(20);
    temp -> setText(formatMessage(msg));
    temp -> setMargin(5);
    temp -> adjustSize();
    temp -> setWordWrap(true);
    QLabel * tempName = new QLabel(chatScreen);
    tempName -> setMaximumWidth(100);
    tempName -> setMinimumHeight(20);
    QString senderName = _senderName;
    tempName ->setText(senderName);

    if (senderId == sender) {
//        tempName -> setStyleSheet("background-color:rgba(245,245,245,250);border:0px;");
//        tempName -> setGeometry(503 - tempName -> width(), offsetHeight, tempName -> width(), tempName -> height());
//        offsetHeight += tempName -> height();
        temp -> setStyleSheet("background-color: #E1F5F2;");
        temp -> setGeometry(503 - temp -> width(), offsetHeight, temp -> width(), temp -> height());
    }
    else {
        tempName -> setStyleSheet("background-color:rgba(245,245,245,250);border:0px;");
        tempName -> setGeometry(20, offsetHeight, tempName -> width(), tempName -> height());
        offsetHeight += tempName -> height();
        temp -> setStyleSheet("background-color: #FFF;");
        temp -> setGeometry(20, offsetHeight, temp -> width(), temp -> height());
    }
    tempName -> show();
    temp -> show();

    offsetHeight += temp -> height() + 10;
    chatScreen -> resize(523, offsetHeight + 15);
}


GrounpChatWidget::~GrounpChatWidget()
{
    delete ui;
}
