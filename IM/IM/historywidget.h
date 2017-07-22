#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QMap>
#include <QLabel>
#include <QVector>
#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QJsonObject>

#include "networkrequest.h"

#define JSON QMap<QString, QString>

namespace Ui {
class HistoryWidget;
}

class HistoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryWidget(QWidget *parent = 0, int _p1 = 0, int _p2 = 0);
    ~HistoryWidget();
    void showThisPage();

public slots:
    void goNextPage();
    void goPrevPage();
    void gotoThatPage(int p);
    void clickOnGotoBtn();

private:
    Ui::HistoryWidget *ui;

    QScrollArea * sa;
    QLabel * screen;
    QLineEdit * pageDisplay;
    QPushButton * nextPage, * prevPage, * gotoPage;

    int p1, p2;
    int cutLen;
    int curPage, all_page;
    QString myName, myFriendName;
    QVector<QJsonObject> allInfo;
    NetworkRequest * nr;
};

#endif // HISTORYWIDGET_H
