#ifndef GROUNPCHATWIDGET_H
#define GROUNPCHATWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPalette>
#include <QPixmap>
#include <QTextEdit>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QScrollArea>

#include "groupinputwidget.h"
#include "networkrequest.h"

#include <QDebug>


namespace Ui {
class GrounpChatWidget;
}

class GrounpChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GrounpChatWidget(QWidget *parent = 0,int _sender = 0);
    ~GrounpChatWidget();
    int getSenderId();
    void appendNewMessage(int senderId, QString _senderName,QString msg);

private:
    Ui::GrounpChatWidget *ui;
    QLabel *chatScreen;//聊天记录
    QWidget * chatScreenWidget;
    QLabel *chatSettingField;//放icon
    QLabel *chatWidgetName;//显示聊天对象用户名
    QScrollArea *chatScrollArea;
    GroupInputWidget * iw;
    QString formatMessage(QString msg);
    int sender;
    int offsetHeight;
    NetworkRequest * nr;
};

#endif // GROUNPCHATWIDGET_H
