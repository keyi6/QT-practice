#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QVector>
#include <QPalette>
#include <QPixmap>
#include <QTextEdit>
#include <QWidget>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QScrollArea>

#include "inputwidget.h"
#include "networkrequest.h"

#include <QDebug>

class QLabel;
class QScrollArea;
class QPushButton;

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = 0, int _receiver = 0,int _sender = 0);

    ~ChatWidget();
    int getSenderId();

    void appendNewMessage(int senderId, QString msg);

private:
    Ui::ChatWidget *ui;
    QLabel *chatScreen;//聊天记录
    QWidget * chatScreenWidget;
    QLabel *chatSettingField;//放icon
    QLabel *chatWidgetName;//显示聊天对象用户名
    QScrollArea *chatScrollArea;
    InputWidget * iw;

    int offsetHeight;
    int sender, receiver;
    QString formatMessage(QString msg);

    NetworkRequest * nr;
};

#endif // CHATWIDGET_H
