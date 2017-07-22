#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QMap>
#include <QFont>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QDialog>
#include <QPainter>
#include <QLineEdit>
#include <QHostInfo>
#include <QJsonObject>
#include <QPushButton>
#include <QKeyEvent>

#include "networkrequest.h"
#include "registerdialog.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    QJsonObject getUserInfo();

private slots:
    void login();
    void registerAsNewUser();

private:
    Ui::LoginDialog * ui;

    QJsonObject user;
    QLabel * usernameLabel, * passwordLabel, * feedback;
    QLineEdit * usernameLineEdit, * passwordLineEdit;
    QPushButton * loginBtn, * registerBtn;

    void keyPressEvent(QKeyEvent * e);
};

#endif // LOGINDIALOG_H
