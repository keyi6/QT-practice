#ifndef SEVER_H
#define SEVER_H

#include <QDialog>
#include <QString>
#include <QFile>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>
#include <QJsonObject>
#include "networkrequest.h"
namespace Ui {
class Sever;
}

class Sever : public QDialog
{
    Q_OBJECT

public:
    explicit Sever(QWidget *parent = 0,int _friendId = 0);
    ~Sever();
    void tcpServerStart();

public slots:
    void showServerDialog();
//    void acceptBtnClicked();
    void updateServerProgress();  //更新进度条，接收数据
    //显示错误
    void displayError(QAbstractSocket::SocketError socketError);

private:
    Ui::Sever *ui;
    QLabel *senderName;
    QLabel *showReceiveFileName;
    QLabel *showCurServerStatus;
//    QPushButton *acceptBtn;//接受接收
//    QPushButton *refuseBtn;//拒绝接收
    QProgressBar *serverProgressBar;
    NetworkRequest *nr;
    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    QString receiveFileName;//接收到的文件名
    QByteArray inBlock;//接收的数据缓冲区
    qint64 totalBytesServer;  //存放总大小信息
    qint64 bytesReceived;  //已收到数据的大小
    qint64 fileNameSize;  //文件名的大小信息
    QFile *localFileReceive;   //本地文件
    int senderId;
    int ReceiverId;
    bool isAccepted;
};

#endif // SEVER_H
