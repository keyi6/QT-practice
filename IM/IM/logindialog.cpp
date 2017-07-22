#include "logindialog.h"
#include "ui_logindialog.h"
#define lineEditSize 200, 25
#define btnSize 75, 30
#define labelSize 300, 20

LoginDialog::LoginDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // this dialog
    QFont font("Microsoft Yahei");
    setFont(font);
    QIcon icon(":/new/prefix1/assets/im.png");
    setWindowIcon(icon);
    setFixedSize(400, 310);
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

    usernameLabel = new QLabel(this);
    usernameLabel -> setText("username");
    usernameLabel -> setGeometry(100, 110, labelSize);

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit -> setFixedSize(lineEditSize);
    usernameLineEdit -> setGeometry(100, 135, lineEditSize);

    passwordLabel = new QLabel(this);
    passwordLabel -> setText("password");
    passwordLabel -> setGeometry(100, 170, labelSize);

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit -> setFixedSize(lineEditSize);
    passwordLineEdit -> setGeometry(100, 195, lineEditSize);
    passwordLineEdit -> setEchoMode(QLineEdit::Password);

    feedback = new QLabel(this);
    feedback -> setText("");
    feedback -> setStyleSheet("color: #FC624D");
    feedback -> setGeometry(100, 240, labelSize);

    loginBtn = new QPushButton(this);
    loginBtn -> setStyleSheet(btnCSS);
    loginBtn -> setGeometry(100, 260, btnSize);
    loginBtn -> setText("Login");
    loginBtn -> setFocusPolicy(Qt::NoFocus);
    connect(loginBtn, SIGNAL(clicked()), this, SLOT(login()));

    registerBtn = new QPushButton(this);
    registerBtn -> setStyleSheet(btnCSS);
    registerBtn -> setGeometry(225, 260, btnSize);
    registerBtn -> setText("register");
    registerBtn -> setFocusPolicy(Qt::NoFocus);
    connect(registerBtn, SIGNAL(clicked()), this, SLOT(registerAsNewUser()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::login() {
    if (usernameLineEdit -> text().length() && passwordLineEdit -> text().length()) {
        QMap<QString, QString> json;
        NetworkRequest * nr = new NetworkRequest;

        json["username"] = usernameLineEdit -> text();
        json["password"] = passwordLineEdit -> text();
        json["ip"] = nr -> getLocalIp();

        user = nr -> post(json, "login");
        QString rCode = user["code"].toString();
        if (rCode.length()) {
            if (rCode[0] != '-')
                emit accept();
            else if (rCode[1] == '1')
                feedback -> setText("No such user!");
            else if (rCode[1] == '2')
                feedback -> setText("Wrong password!");
            else
                feedback -> setText("Network Error!");
        }
        else
            feedback -> setText("Network Error!");
    }
    else
        feedback -> setText("Fill all the black please");
}

void LoginDialog::registerAsNewUser() {
    RegisterDialog rd;
    rd.exec();
}

QJsonObject LoginDialog::getUserInfo() {
    return user;
}

void LoginDialog::keyPressEvent(QKeyEvent * e) {
    if (e -> key() == Qt::Key_Enter || e -> key() == 16777220)
        login();
}
