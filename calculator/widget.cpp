#include "widget.h"
#include "ui_widget.h"
#include "expression.h"

Widget::Widget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Widget)
{
    ui->setupUi(this);

    curExpression = "";
    recordHistory = "HISTORY";
    int offsetX = 0, offsetY = 160, height = 160 + 60 * 7;

    resize(QSize(360, height));
    setMinimumSize(360, height);
    setMaximumSize(720, height);
    setStyleSheet("background-color: #E2E2E2; color: #000; font-size: 20px;");
    setWindowTitle(QString("Calculator v1.0 ©Kim Lee"));
    setWindowIcon(QIcon("app.ico"));

    for (int i = 0; i < 7; i ++)
        for (int j = 0; j < 4; j ++)
        {
            pushButton[i][j] = new QPushButton(this);
            pushButton[i][j] -> setText(Ui::keyText[i][j]);
            pushButton[i][j] -> setGeometry(90 * j + offsetX, 60 * i + offsetY, 90, 60);
            pushButton[i][j] -> setStyleSheet("background-color: " + Ui::bgColor[i][j] + "; color: " + Ui::ftColor[i][j] + "; border: 0px; QPushButton{text-align : center;}");
            connect(pushButton[i][j], SIGNAL(clicked()), this, SLOT(keyPress()));
        }

    screen = new QLabel(this);
    screen -> setGeometry(0, 0, 360, offsetY);
    screen -> setStyleSheet("font-size: 35px; background-color: #E2E2E2; color: #211F1F;");
    screen -> setMaximumSize(360, offsetY);
    screen -> setMinimumSize(360, offsetY);
    screen -> setMargin(20);

    historyScrollArea = new QScrollArea(this);
    historyScrollArea -> setStyleSheet("background-color: #E2E2E2; color: #211F1F; border: 0px;");
    historyScrollArea -> setGeometry(360, 0, 360, height);
    historyScrollArea -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    historyScrollArea -> setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    historyWidget = new QWidget(historyScrollArea);
    historyWidget -> setGeometry(360, 0, 350, 2000);
    historyWidget -> setStyleSheet("background-color: #E2E2E2; color: #211F1F; border: 0px;");
    historyScrollArea -> setWidget(historyWidget);

    history = new QLabel(historyWidget);
    history -> setGeometry(0, 0, 340, 2000);
    history -> setText(recordHistory);
    history -> setAlignment(Qt::AlignLeft);
    history -> setMargin(20);
    history -> setStyleSheet("background-color: #E2E2E2; color: #211F1F");
    history -> setMinimumSize(340, 2000);
    history -> setMaximumSize(340, 2000);
}

void Widget::calculate()
{
    // auto fill ()
    int temp_cnt = 0;
    for (int i = 0; i < curExpression.length(); i ++)
        if (curExpression[i] == '(')
            ++ temp_cnt;
        else if (curExpression[i] == ')')
            -- temp_cnt;
    while (temp_cnt --)
        curExpression += ")";

    int t;
    bool flag;
    QString temp = curExpression;

    // format expression
    temp.replace(" ", "");
    temp.replace("\n", "");
    temp.replace("²", "^2");
    temp.replace("÷", "/");
    temp.replace("×", "*");

    // consecutive calculation
    t = temp.indexOf("=");
    if (~t)
    {
        bool consecutiveCal = false;
        for (int i = t + 1; i < temp.length(); i ++)
            if (!('0' <= temp[i] && temp[i] <= '9') && temp[i] != '.')
            {
                if (temp[i] == '-' && i == t + 1)
                    continue;
                consecutiveCal = true;
                break;
            }

        if (consecutiveCal)
        {
            temp = temp.mid(t + 1, (int)temp.length() - t - 1);
            curExpression = temp;
        }
        else
        {
            temp = temp.mid(0, t);
            temp.replace("\n", "");
            flag = false;
            for (int i = temp.length() - 1; i >= 0 && !flag; i --)
            {
                if (temp[i] == '+' || temp[i] == '*' || temp[i] == '/')
                {
                    temp = temp + temp.mid(i, temp.length() - i);
                    curExpression = temp;
                    flag = true;
                }
                else if (temp[i] == '-')
                {
                    if (temp[i - 1] == '+' || temp[i - 1] == '-' || temp[i - 1] == '*' || temp[i - 1] == '/')
                        i --;

                    temp = temp + temp.mid(i, temp.length() - i);
                    curExpression = temp;
                    flag = true;
                }
            }

            if (!flag)
                return;
        }
    }

    t = temp.indexOf("%");
    while (~t)
    {
        temp.replace("%", "");
        flag = false;

        for (int i = t - 1, j; i >= 0; i --)
            if (temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/')
            {
                for (j = i - 1; j >= 0; j --)
                    if (temp[j] == '+' || temp[j] == '-' || temp[j] == '*' || temp[j] == '/')
                        break;

                if (~j)
                    temp = temp.mid(0, i + 1) + temp.mid(j + 1, i - j - 1) + "*" + temp.mid(i + 1, t - i- 1) + "/100" + temp.mid(t, temp.length()- t);
                else
                    temp = temp.mid(0, i + 1) + temp.mid(0, i) + "*" + temp.mid(i + 1, t - i - 1) + "/100" + temp.mid(t, temp.length() - t);

                flag = true;
                break;
            }

        if (! flag) {
            curExpression = "Invalid Expression!";
            break;
        }

        t = temp.indexOf("%");
    }

    t = temp.indexOf("√");
    while (~t)
    {
        temp.replace("√", "");
        flag = false;
        if (temp[t] == '(')
        {
            int cnt = 0;
            for (int i = t; i < temp.length() && !flag; i ++)
                if (temp[i] == '(')
                    cnt ++;
                else if (temp[i] == ')' && !(-- cnt))
                {
                    temp = temp.mid(0, i + 1) + "^0.5" + temp.mid(i + 1, temp.length() - i - 1);
                    qDebug() << temp;
                    flag = true;
                }
        }
        else
        {
            if (temp[t] == '-')
            {
                curExpression += "\n=Invalid Expression!";
                return;
            }
            for (int i = t; i < temp.length() && !flag; i ++)
                if (!('0' <= temp[i] && temp[i] <= '9') && temp[i] != '.')
                {
                    temp = temp.mid(0, i) + "^0.5" + temp.mid(i, temp.length() - i),
                                    flag = true;
                }
        }

        if (!flag)
            temp += "^0.5";
        t = temp.indexOf("√");
    }

    Expression e(temp.toStdString());
    QString res = QString::fromStdString(e.getValue());

    qDebug() << temp << res << endl;
    curExpression += "\n=" + res;

    temp = curExpression;
    temp.replace("\n", "");
    recordHistory += "\n\n" + temp;
    history -> setText(recordHistory);
}

void Widget::updateExpression(QString exp)
{
    curExpression.replace("×", "*");
    curExpression.replace("÷", "/");
    if (exp == "=")
        calculate();
    else if (exp == "◁")
    {
        if (curExpression.length())
            curExpression = curExpression.mid(0, curExpression.length() - 1);
    }
    else if (exp == "⅟ x")
    {
        if (curExpression.length())
        {
            bool flag = false;
            int cnt = 0;
            for (int i = curExpression.length() - 1 && !flag; i > 0; i --)
            {
                if (curExpression[i] == '+' || curExpression[i] == '-' || curExpression[i] == '*' || curExpression[i] == '/')
                    curExpression = curExpression.mid(0, i + 1) + "1/" + curExpression.mid(i + 1, curExpression.length() - i - 1),
                                    flag = true;
                else if (curExpression[i] == ')')
                    ++ cnt;
                else if (curExpression[i] == '(')
                {
                    -- cnt;
                    if (! cnt)
                        curExpression = curExpression.mid(0, i + 1) + "1/" + curExpression.mid(i + 1, curExpression.length() - i - 1),
                                        flag = true;
                }
            }

            if (!flag)
                curExpression = "1/" + curExpression;
        }
        else
            curExpression = "Invalid Expression!";
    }
    else if (exp == "±")
    {

        if (curExpression.length() && curExpression != "-")
        {
            bool flag = false;
            for (int i = curExpression.length() - 1; ! flag && i >= 0; i --)
            {
                if (curExpression[i] == '+' || curExpression[i] == '*' || curExpression[i] == '/')
                {
                    curExpression = curExpression.mid(0, i + 1) + "-" + curExpression.mid(i + 1, curExpression.length() - i - 1);
                    flag = true;
                    break;
                }
                else if (curExpression[i] == '-')
                {
                    if (!i)
                        curExpression = curExpression.mid(1, curExpression.length() - 1);
                    else
                    {
                        if (! ('0' <= curExpression[i - 1] && curExpression[i - 1] <= '9') && curExpression[i - 1] != '.')
                            curExpression = curExpression.mid(0, i) + curExpression.mid(i + 1, curExpression.length() - i - 1);
                        else
                            curExpression = curExpression.mid(0, i + 1) + "-" + curExpression.mid(i + 1, curExpression.length() - i - 1);
                    }

                    flag = true;
                    break;
                }
            }

            if (!flag)
                curExpression = "-" + curExpression;
        }
        else if (curExpression == "-")
            curExpression = "";
        else
            curExpression = "-";
    }
    else if (exp == "x²")
        curExpression += "²";
    else if (exp == "C")
        curExpression = "";
    else if (exp == "History")
    {
        if (this -> width() == 360)
            resize(720, 580);
        else
            resize(360, 580);
    }
    else if (exp == "CE")
    {
        bool flag = false;
        for (int i = curExpression.length() - 1; i >= 0 && ! flag; i --)
            if (!('0' <= curExpression[i] && curExpression[i] <= '9') && curExpression[i] != '.')
            {
                if (i == curExpression.length() - 1)
                    continue;

                curExpression = curExpression.mid(0, i + 1);
                flag = true;
            }
        if (! flag)
            curExpression = "";
    }
    else
        curExpression += exp;

    curExpression.replace("*", "×");
    curExpression.replace("/", "÷");
    screen -> setText(curExpression);
}

void Widget::keyPressEvent(QKeyEvent * e)
{
    int key = e -> key();
    if (('0' <= key && key <= '9') || key == '.' || key == '+' || key == '-' || key == '*' || key == '/' || key == '(' || key == ')' || key == '%')
        curExpression += key;
    else if (key == Qt::Key_Backspace)
    {
        if (curExpression.length())
            curExpression = curExpression.mid(0, curExpression.length() - 1);
    }
    else if (key == Qt::Key_Enter || key == 16777220)
        calculate();

    curExpression.replace("*", "×");
    curExpression.replace("/", "÷");
    screen -> setText(curExpression);
}

void Widget::keyPress()
{
    QPushButton * pressedKey = qobject_cast<QPushButton *>(sender());
    for (int i = 0; i < 7; i ++)
        for (int j = 0; j < 4; j ++)
            if (pressedKey == pushButton[i][j])
            {
                updateExpression(Ui::keyText[i][j]);
                break;
            }
}

Widget::~Widget()
{
    delete ui;
}
