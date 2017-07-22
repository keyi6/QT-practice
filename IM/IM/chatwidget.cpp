#include "chatwidget.h"
#include "ui_chatwidget.h"

ChatWidget::ChatWidget(QWidget *parent, int _receiver, int _sender) :
    QWidget(parent),
    sender(_sender),
    receiver(_receiver),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
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
    QJsonObject myFriend = nr -> getUserInfoById(_receiver);
    chatWidgetName -> setText(myFriend["username"].toString());

    chatScreen = new QLabel(this);
    chatScreen -> setFixedWidth(523);
    chatScreen -> setGeometry(0, 30, 523, 10);
    chatScreen -> setFocusPolicy(Qt::NoFocus);
    chatScreen -> adjustSize();
    chatScrollArea -> setWidget(chatScreen);

    iw = new InputWidget(this, sender, receiver,myFriend["ip"].toString());
    qDebug()<<myFriend;
    iw -> setGeometry(0, 430, 540, 200);
}

int ChatWidget::getSenderId(){
    return sender;
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

QString ChatWidget::formatMessage(QString msg) {
    int cutLen = 48;
    QString ret = "";
    for (int i = 0; i < msg.length(); i += cutLen)
        ret += msg.mid(i, cutLen) + "\n";


    ret = ret.mid(0, ret.length() - 1);
    return ret;
}

void ChatWidget::appendNewMessage(int senderId, QString msg) {
    QLabel * temp = new QLabel(chatScreen);
    temp -> setMaximumWidth(400);
    temp -> setMinimumHeight(20);
    temp -> setText(formatMessage(msg));
    temp -> setMargin(5);
    temp -> adjustSize();
    temp -> setWordWrap(true);

    if (senderId == sender) {
        temp -> setStyleSheet("background-color: #E1F5F2;");
        temp -> setGeometry(503 - temp -> width(), offsetHeight, temp -> width(), temp -> height());
    }
    else {
        temp -> setStyleSheet("background-color: #FFF;");
        temp -> setGeometry(20, offsetHeight, temp -> width(), temp -> height());
    }

    temp -> show();

    offsetHeight += temp -> height() + 10;
    chatScreen -> resize(523, offsetHeight + 15);

}
