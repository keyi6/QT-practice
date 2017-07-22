#include "client.h"
#include "ui_client.h"
#include <QDebug>

Client::Client(QWidget *parent,QString _friendIp) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    fileName = "";
    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    tcpClient = new QTcpSocket(this);
    receiverIp = _friendIp;
   // nr = new NetworkRequest;
    showFileName = new QLabel(this);
    showFileName -> setGeometry(30,90,350,30);
    showFileName -> setStyleSheet("background-color:#fff;color:#333;border:1px solid grey;");
    showFileName -> setText(fileName);
    chooseFileBtn = new QPushButton("select",this);
    chooseFileBtn -> setGeometry(30,30,50,30);
    chooseFileBtn -> setStyleSheet("background-color:#fff;color:#333;border:1px solid grey;");
    connect(chooseFileBtn,SIGNAL(clicked()),this,SLOT(getFileName()));
    showCurStatus = new QLabel(this);
    showCurStatus -> setGeometry(30,60,300,30);
    showCurStatus -> setStyleSheet("color:#333;border:0px;");
    showCurStatus -> setText("waiting for selecting");

    clientProgressBar = new QProgressBar(this);
    clientProgressBar -> setGeometry(50,150,300,30);
    clientProgressBar -> setRange(0,100);
    clientProgressBar -> setValue(0);
    clientProgressBar -> setStyleSheet("QProgressBar::chunk {background-color: rgba(252,98,77,180);border-radius: 5px;}QProgressBar {background-color:#fff;border:0px; border-radius: 5px;   text-align: center;}");

    sendBtn = new QPushButton("Send",this);
    sendBtn -> setEnabled(false);
    sendBtn -> setGeometry(100,210,50,30);
    sendBtn -> setStyleSheet("background-color:rgba(252,98,77,180);color:#fff;border-radius: 5px;border:0px;");
    connect(sendBtn,SIGNAL(clicked()),this,SLOT(send()));
    closeBtn = new QPushButton("Close",this);
    closeBtn -> setGeometry(250,210,50,30);
    closeBtn -> setStyleSheet("background-color:rgba(252,98,77,180);color:#fff;border-radius: 5px;border:0px;");
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(close()));

    //当连接服务器成功时，发出connected()信号，我们开始传送文件
    connect(tcpClient,SIGNAL(connected()),this,SLOT(startTransfer()));
    //当有数据发送成功时，我们更新进度条
    connect(tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
}
void Client::getFileName(){
    fileName = QFileDialog::getOpenFileName(this,"select file");
    showFileName -> setText(fileName);
    showCurStatus -> setText("waiting for send");
    sendBtn -> setEnabled(true);
}
void Client::send(){
    sendBtn -> setEnabled(false);
    bytesWritten = 0;
    showCurStatus -> setText("is connecting to Host");
   // receiverIp = nr -> getUserInfoById(receiverId)["ip"].toString();
    tcpClient -> connectToHost(QHostAddress(receiverIp),8453);
    qDebug() << receiverIp;
//    tcpClient -> connectToHost(QHostAddress("192.168.47.192"),8453);
 //   tcpClient -> connectToHost(QHostAddress::LocalHost,8453);
}

void Client::startTransfer(){
    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly)){
           qDebug() << "open file error!";
           return;
    }
    totalBytes = localFile -> size();

    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_9);
    QString currentFileName = fileName.right(fileName.size()- fileName.lastIndexOf('/')-1);

    //依次写入总大小信息空间，文件名大小信息空间，文件名
    sendOut << qint64(0) << qint64(0) << currentFileName;
    //这里的总大小是文件名大小等信息和实际文件大小的总和
    totalBytes += outBlock.size();

    sendOut.device()->seek(0);
    //返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));
    //发送完头数据后剩余数据的大小
    bytesToWrite = totalBytes - tcpClient->write(outBlock);
    showCurStatus->setText("connected");
    outBlock.resize(0);
}

//更新进度条，实现文件的传送
void Client::updateClientProgress(qint64 numBytes)
{
    //已经发送数据的大小
    bytesWritten += (int)numBytes;

    if(bytesToWrite > 0) //如果已经发送了数据
    {
   //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
   //就发送剩余数据的大小
       outBlock = localFile->read(qMin(bytesToWrite,loadSize));

       //发送完一次数据后还剩余数据的大小
       bytesToWrite -= (int)tcpClient->write(outBlock);

       //清空发送缓冲区
       outBlock.resize(0);

    } else {
       localFile->close(); //如果没有发送任何数据，则关闭文件
    }

    //更新进度条
    clientProgressBar->setMaximum(totalBytes);
    clientProgressBar->setValue(bytesWritten);

    if(bytesWritten == totalBytes) //发送完毕
    {
       showCurStatus->setText(tr("send successfully").arg(fileName));
       localFile->close();
       tcpClient->close();
    }
}


void Client::displayError(QAbstractSocket::SocketError) //显示错误
{
    qDebug() << tcpClient->errorString();
    tcpClient->close();
    clientProgressBar->reset();
    showCurStatus->setText("client ready");
    sendBtn->setEnabled(true);
}

Client::~Client()
{
    delete ui;
}
