#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QTcpSocket>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QDataStream>
#include <QByteArray>
#include <QHostAddress>
#include "networkrequest.h"

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0,QString _friendIp = "");
    ~Client();

private:
    Ui::Client *ui;
    QTcpSocket *tcpClient;

    QFile *localFile;//本地文件
    qint64 totalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    QString fileName;  //文件路径
    QString receiverIp;
    QByteArray outBlock;
    int receiverId;
    QLabel *showCurStatus;
    QLabel *showFileName;//展示传输文件路径
    QProgressBar *clientProgressBar;//进度条
    QPushButton *chooseFileBtn;
    QPushButton *sendBtn;
    QPushButton *closeBtn;
//    NetworkRequest *nr;

private slots:
    void getFileName();
    void send();  //连接服务器
    void startTransfer();  //发送文件大小等信息
    void updateClientProgress(qint64); //发送数据，更新进度条
    void displayError(QAbstractSocket::SocketError); //显示错误
};

#endif // CLIENT_H
