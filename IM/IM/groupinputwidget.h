#ifndef GROUPINPUTWIDGET_H
#define GROUPINPUTWIDGET_H

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

namespace Ui {
class GroupInputWidget;
}

class GroupInputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupInputWidget(QWidget *parent = 0,int _sender = 0);
    ~GroupInputWidget();
public slots:
    void sendGroupMessage();
private:
    Ui::GroupInputWidget *ui;
    int sender;
    QPushButton *sendTextBtn;
    QTextEdit *inputField;//输入框
    QString curInputContent;
    QUdpSocket * UdpSender;

    QString JSONtoJSONString(JSON data);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // GROUPINPUTWIDGET_H
