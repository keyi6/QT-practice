#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QColor>
#include <QLabel>
#include <QVector>
#include <QWidget>
#include <QPixmap>
#include <QJsonObject>
#include <QPushButton>


#include <QDebug>

namespace Ui {
class FriendList;
}

class FriendList : public QWidget
{
    Q_OBJECT

public:
    explicit FriendList(QWidget *parent = 0);
    ~FriendList();

    void addNewFriendToList(QJsonObject newFriend);
    void getNewMessage(QJsonObject sender);
    void removeFriendFromList(int fid);

public slots:
    void showChatWidget();

signals:
    void friendIdSignal(int i);
    void showOneChatWidget(int i);
    void friendIdSignalWhenReceivingNewMessage(int i, int sender, QString msg);
    void showGroupWidget();

private:
    int fixedHeight = 80;
    Ui::FriendList * ui;

    QPushButton * gBtn;

    static QString avatarBackgroundColor[6];
    int friendId;
    QVector<int> unread;
    QVector<QJsonObject> friendList;
    QVector<QLabel *> friendListAvator, friendListInfo, messageUnread;
    QVector<QPushButton *> friendButton;
};

#endif // FRIENDLIST_H
