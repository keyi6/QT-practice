#include "registerdialog.h"
#include "ui_registerdialog.h"
#define lineEditSize 200, 25
#define btnSize 100, 30
#define labelSize 300, 20

RegisterDialog::RegisterDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::RegisterDialog) {
    ui->setupUi(this);

    // this dialog
    QFont font("Microsoft Yahei");
    setFont(font);
    QIcon icon(":/new/prefix1/assets/im.png");
    setWindowIcon(icon);
    setFixedSize(400, 370);
    setAutoFillBackground(true);
    setPalette(Qt::white);
    setWindowTitle("IM v1.0 © chili");

    QLabel * decorationBackground = new QLabel(this);
    decorationBackground -> setText("IM");
    decorationBackground -> setStyleSheet("font-size: 40px; color: #FFF;");
    decorationBackground -> setAlignment(Qt::AlignCenter);
    decorationBackground -> setFixedSize(400, 100);
    decorationBackground -> setAutoFillBackground(true);
    decorationBackground -> setGeometry(0, 0, 400, 100);
    decorationBackground -> setPalette(QColor("#FC624D"));

    QString btnCSS = "background-color: #FC624D; color: #FFF; border: 0px;";

    QLabel * usernameLabel = new QLabel(this);
    usernameLabel -> setText("please input your username");
    usernameLabel -> setGeometry(100, 110, labelSize);

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit -> setFixedSize(lineEditSize);
    usernameLineEdit -> setGeometry(100, 135, lineEditSize);

    QLabel * passwordLabel = new QLabel(this);
    passwordLabel -> setText("please input your password");
    passwordLabel -> setGeometry(100, 170, labelSize);

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit -> setFixedSize(lineEditSize);
    passwordLineEdit -> setGeometry(100, 195, lineEditSize);
    passwordLineEdit -> setEchoMode(QLineEdit::Password);
    connect(passwordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkPassword()));

    QLabel * passwordRepeatLabel = new QLabel(this);
    passwordRepeatLabel -> setText("please repeat your password");
    passwordRepeatLabel -> setGeometry(100, 230, labelSize);

    passwordRepeatLineEdit = new QLineEdit(this);
    passwordRepeatLineEdit -> setFixedSize(lineEditSize);
    passwordRepeatLineEdit -> setGeometry(100, 255, lineEditSize);
    passwordRepeatLineEdit -> setEchoMode(QLineEdit::Password);
    connect(passwordRepeatLineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkPassword()));

    feedback = new QLabel(this);
    feedback -> setText("");
    feedback -> setStyleSheet("color: #FC624D");
    feedback -> setGeometry(100, 300, labelSize);

    registerBtn = new QPushButton(this);
    registerBtn -> setStyleSheet(btnCSS);
    registerBtn -> setGeometry(150, 320, btnSize);
    registerBtn -> setText("register");
    registerBtn -> setFocusPolicy(Qt::NoFocus);
    registerBtn -> setCursor(Qt::PointingHandCursor);
    connect(registerBtn, SIGNAL(clicked()), this, SLOT(registerAsNewUser()));
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::checkPassword() {
    QString btnCSS = "background-color: #FC624D; color: #FFF; border: 0px;";
    QString badBtnCSS = "background-color: #FFA093; color: #FFF; border: 0px;";

    if (passwordLineEdit -> text() == passwordRepeatLineEdit -> text() &&
                    passwordLineEdit -> text().length()) {
        registerBtn -> setEnabled(true);
        registerBtn -> setStyleSheet(btnCSS);
        registerBtn -> setCursor(Qt::PointingHandCursor);

        feedback -> setText("");
    }
    else {
        registerBtn -> setEnabled(false);
        registerBtn -> setStyleSheet(badBtnCSS);
        registerBtn -> setCursor(Qt::ForbiddenCursor);

        feedback -> setText("passwords were not the same!");
    }
}

void RegisterDialog::registerAsNewUser() {
    if (passwordLineEdit -> text() == passwordRepeatLineEdit -> text() && passwordLineEdit -> text().length()) {
        JSON data;
        data["username"] = usernameLineEdit -> text();
        data["password"] = passwordLineEdit -> text();

        NetworkRequest nr;
        QJsonObject res = nr.post(data, "register");
        QString rCode = res["code"].toString();

        if (rCode[0] != '-')
            emit accept();
        else if (rCode[1] == '2')
            feedback -> setText("username already been taken!");
        else
            feedback -> setText("Network Error!");
    }

}
