#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QMap>
#include <QLabel>
#include <QString>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkReply>

#include "networkrequest.h"

#define JSON QMap<QString, QString>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = 0);
    ~RegisterDialog();

private slots:
    void registerAsNewUser();
    void checkPassword();

private:
    Ui::RegisterDialog *ui;

    QLabel * feedback;
    QPushButton * registerBtn;
    QLineEdit * usernameLineEdit, * passwordLineEdit, * passwordRepeatLineEdit;
};

#endif // REGISTERDIALOG_H
