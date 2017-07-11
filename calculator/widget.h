#ifndef WIDGET_H
#define WIDGET_H

#include <QLabel>
#include <QString>
#include <QWidget>
#include <QKeyEvent>
#include <QScrollArea>
#include <QPushButton>

#include <QDebug>

namespace Ui {
class Widget;
const int maxn = 7;
const QString keyText[maxn][maxn] = {
    {"(", ")", "^", "History"},
    {"%", "√", "x²", "⅟ x"},
    {"C", "CE", "◁", "÷"},
    {"7", "8", "9", "×"},
    {"4", "5", "6", "-"},
    {"1", "2", "3", "+"},
    {"±", "0", ".", "="}
};

const QString ftColor[maxn][maxn] = {
    {"#505050", "#505050", "#505050", "#505050"},
    {"#505050", "#505050", "#505050", "#505050"},
    {"#505050", "#505050", "#505050", "#00C61B"},
    {"#211F1F", "#211F1F", "#211F1F", "#00C61B"},
    {"#211F1F", "#211F1F", "#211F1F", "#00C61B"},
    {"#211F1F", "#211F1F", "#211F1F", "#00C61B"},
    {"#211F1F", "#211F1F", "#211F1F", "#E2E2E2"}
};

const QString bgColor[maxn][maxn] = {
    {"#E2E2E2", "#E2E2E2", "#E2E2E2", "#E2E2E2"},
    {"#E2E2E2", "#E2E2E2", "#E2E2E2", "#E2E2E2"},
    {"#E2E2E2", "#E2E2E2", "#E2E2E2", "#E2E2E2"},
    {"#E2E2E2", "#E2E2E2", "#E2E2E2", "#E2E2E2"},
    {"#E2E2E2", "#E2E2E2", "#E2E2E2", "#E2E2E2"},
    {"#E2E2E2", "#E2E2E2", "#E2E2E2", "#E2E2E2"},
    {"#E2E2E2", "#E2E2E2", "#E2E2E2", "#00C61B"}
};
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void keyPress();
    void keyPressEvent(QKeyEvent * e);

private:
    Ui::Widget *ui;

    QWidget * historyWidget;
    QLabel * screen, * history;
    QScrollArea * historyScrollArea;
    QString curExpression, recordHistory;
    QPushButton * pushButton[Ui::maxn][Ui::maxn];

    void calculate();
    void updateHistory();
    void updateExpression(QString exp);
};

#endif // WIDGET_H
