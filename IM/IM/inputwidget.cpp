#include "inputwidget.h"
#include "ui_inputwidget.h"

InputWidget::InputWidget(QWidget *parent, int _sender, int _receiver,QString _friendIp) :
        QWidget(parent),
        sender(_sender),
        receiver(_receiver),
        ui(new Ui::InputWidget)
{
    ui->setupUi(this);

    setStyleSheet("border: 0px;");
    curInputContent = "";
    fileName = "";
    friendIp = _friendIp;
    chatSettingField = new QLabel(this);
    chatSettingField -> setGeometry(0, 0, 540, 30);
    chatSettingField -> setFocusPolicy(Qt::NoFocus);
    chatSettingField -> setStyleSheet("background-color:rgba(245,245,245,250);border-top: 1px solid rgba(235,235,235,250);");

    fileBtn = new QPushButton("File",this);
    fileBtn -> setGeometry(0,0,50,30);
    fileBtn -> setFocusPolicy(Qt::NoFocus);
    fileBtn -> setStyleSheet("background-color:rgba(245,245,245,250); color: #333333;border-right: 1px solid rgba(235,235,235,250);border-top: 1px solid rgba(235,235,235,250) ;");
    serverDialog = new Sever(this,_receiver);
    connect(fileBtn,SIGNAL(clicked()),this,SLOT(createClient()));

    inputField = new QTextEdit(this);
    inputField -> setGeometry(0, 30, 540, 170);
    inputField -> setStyleSheet("background-color:rgba(250,250,250,250);border: 0px;");
    inputField -> installEventFilter(this);

    sendTextBtn = new QPushButton("0K",this);
    sendTextBtn -> setGeometry(470,150,50,30);
    sendTextBtn -> setFocusPolicy(Qt::NoFocus);
    sendTextBtn -> setStyleSheet("background-color:rgba(250,250,250,250);border:1px solid rgba(235,235,235,250);");
    connect(sendTextBtn,SIGNAL(clicked()),this,SLOT(sendInputContent()));

    historyBtn = new QPushButton("History", this);
    historyBtn -> setGeometry(50,0,50,30);
    historyBtn -> setFocusPolicy(Qt::NoFocus);
    historyBtn -> setStyleSheet("background-color:rgba(245,245,245,250); color: #333333;border-right: 1px solid rgba(235,235,235,250);border-top: 1px solid rgba(235,235,235,250) ;");
    connect(historyBtn, SIGNAL(clicked()), this, SLOT(showHistory()));

    UdpSender = new QUdpSocket;
}

void InputWidget::sendInputContent(){
    curInputContent = inputField -> toPlainText();
    if (curInputContent.length()) {
        JSON data;

        curInputContent.replace("\\", "\\\\");
        curInputContent.replace("\"", "\\\"");
        data["msg"] = curInputContent;
        data["sender"] = QString::number(sender);
        data["receiver"] = QString::number(receiver);

        QByteArray datagram;
        datagram.append(JSONtoJSONString(data).toUtf8());
        qDebug() << "[SEND DATA]" << datagram << JSONtoJSONString(data);
        UdpSender -> writeDatagram(datagram.data(), datagram.size(), QHostAddress(NetworkRequest::hostGateWay), 8322);

        inputField -> clear();
    }
}


/* create sendFileWidget */
void InputWidget::createClient(){
    clientDialog = new Client(this,friendIp);
    clientDialog -> exec();
}

InputWidget::~InputWidget()
{
    delete ui;
}

QString InputWidget::JSONtoJSONString(JSON data) {
    QString ret = "{ ";
    for (JSON::iterator it = data.begin(); it != data.end(); it ++)
        ret += "\"" + it.key() + "\": \"" + it.value() + "\",";

    ret = ret.mid(0, ret.length() - 1);
    ret += " }";
    return ret;
}

bool InputWidget::eventFilter(QObject * watched, QEvent * event) {
    if (watched == inputField) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                sendInputContent();
                return true;
            }
        }
    }
    return false;
}

void InputWidget::showHistory() {
    HistoryWidget * hw = new HistoryWidget(NULL, sender, receiver);
    hw -> show();
}
