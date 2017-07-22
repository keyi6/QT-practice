#include "index.h"
#include "logindialog.h"

#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    LoginDialog * ld = new LoginDialog;

    if (ld -> exec() == QDialog::Accepted) {
        Index w(NULL, ld -> getUserInfo());
        w.show();
        return a.exec();
    }
    else
        return 0;
}
