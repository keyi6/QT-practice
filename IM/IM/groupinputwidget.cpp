#include "groupinputwidget.h"
#include "ui_groupinputwidget.h"

GroupInputWidget::GroupInputWidget(QWidget *parent, int _sender) :
    QWidget(parent),
    sender(_sender),
    ui(new Ui::GroupInputWidget)
{
    ui->setupUi(this);
    setStyleSheet("border: 0px;");
    curInputContent = "";

    inputField = new QTextEdit(this);
    inputField -> setGeometry(0, 0, 540, 183);
    inputField -> setStyleSheet("background-color:rgba(250,250,250,250);border: 0px;");
    inputField -> installEventFilter(this);

    sendTextBtn = new QPushButton("0K",this);
    sendTextBtn -> setGeometry(470,140,50,30);
    sendTextBtn -> setFocusPolicy(Qt::NoFocus);
    sendTextBtn -> setStyleSheet("background-color:rgba(250,250,250,250);border:1px solid rgba(235,235,235,250);");
    connect(sendTextBtn,SIGNAL(clicked()),this,SLOT(sendGroupMessage()));

    UdpSender = new QUdpSocket;
}

GroupInputWidget::~GroupInputWidget()
{
    delete ui;
}
void GroupInputWidget::sendGroupMessage(){
    curInputContent = inputField -> toPlainText();
    if (curInputContent.length()) {
        JSON data;

        curInputContent.replace("\\", "\\\\");
        curInputContent.replace("\"", "\\\"");
        //data["msg"] = curInputContent;
        data["groupMsg"] = curInputContent;
        data["sender"] = QString::number(sender);

        QByteArray datagram;
        datagram.append(JSONtoJSONString(data).toUtf8());
        qDebug() << "[SEND DATA]" << datagram << JSONtoJSONString(data);
        UdpSender -> writeDatagram(datagram.data(), datagram.size(), QHostAddress(NetworkRequest::hostGateWay), 8322);

        inputField -> clear();
    }
}
QString GroupInputWidget::JSONtoJSONString(JSON data) {
    QString ret = "{ ";
    for (JSON::iterator it = data.begin(); it != data.end(); it ++)
        ret += "\"" + it.key() + "\": \"" + it.value() + "\",";

    ret = ret.mid(0, ret.length() - 1);
    ret += " }";
    return ret;
}
bool GroupInputWidget::eventFilter(QObject * watched, QEvent * event) {
    if (watched == inputField) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                sendGroupMessage();
                return true;
            }
        }
    }
    return false;
}
