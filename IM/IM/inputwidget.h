#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QFontComboBox>
#include <QToolButton>
#include <QTextEdit>
#include <QString>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QUdpSocket>
#include <QMap>
#include <QKeyEvent>
#include <QByteArray>
#include <QTcpServer>
#include <QDebug>

#include "client.h"
#include "sever.h"
#include "networkrequest.h"
#include "historywidget.h"

#define JSON QMap<QString, QString>

namespace Ui {
class InputWidget;
}

class InputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InputWidget(QWidget *parent = 0, int _sender = 0, int _receiver = 0,QString _friendIp = "");
    ~InputWidget();

public slots:
    void sendInputContent();
    void createClient();
    void showHistory();

private:
    int sender, receiver;

    Ui::InputWidget *ui;
    Client *clientDialog;
    Sever *serverDialog;

    QLabel *chatSettingField;
    QPushButton *fileBtn;
    QPushButton *sendTextBtn, * historyBtn;
    QTextEdit *inputField;//输入框
    QString curInputContent;
    QString fileName;
    QString friendIp;
    QUdpSocket * UdpSender;

    QString JSONtoJSONString(JSON data);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

};

#endif // INPUTWIDGET_H

