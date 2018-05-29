#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Widget) {
    ui->setupUi(this);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    setStyleSheet("background-color: #E2E2E2;");
    setWindowTitle("QPaintist v1.0 ©Kim Lee");

    int offsetX = 30, btnHeight = 30;

    penColor = Qt::black, penWeight = 1;

    canvas = new Canvas;
    setCentralWidget(canvas);
    canvas -> setGeometry(offsetX, 0, 1366, 768);

    for (int i = 0; i < Ui::maxn; i ++) {
        clickedStatus[i] = 0;

        QIcon temp;
        temp.addFile(":/new/prefix1/assets/" + Ui::altInfo[i] + ".png");

        iconBtn[i] = new QPushButton(this);
        iconBtn[i] -> setGeometry(0, i * btnHeight, offsetX, btnHeight + 1);
        iconBtn[i] -> setIcon(temp);
        iconBtn[i] -> setStyleSheet("background-color: #FFF; border: 0px;");
        iconBtn[i] -> setAccessibleDescription(Ui::altInfo[i]);
        if (i > 5)
            iconBtn[i] -> setToolTip(Ui::altInfo[i]);
        else
            iconBtn[i] -> setToolTip("Draw " + Ui::altInfo[i]);

        if (i == 0)
            connect(iconBtn[i], SIGNAL(clicked()), this, SLOT(drawLine()));
        if (i == 1)
            connect(iconBtn[i], SIGNAL(clicked()), this, SLOT(drawSquare()));
        if (i == 2)
            connect(iconBtn[i], SIGNAL(clicked()), this, SLOT(drawCircle()));
        if (i == 3)
            connect(iconBtn[i], SIGNAL(clicked()), this, SLOT(drawPolygon()));
        if (i == 4) {
            connect(iconBtn[i], SIGNAL(clicked()), this, SLOT(changePenColor()));
            iconBtn[i] -> setAutoFillBackground(true);
            iconBtn[i] -> setPalette(penColor);
        }
        if (i == 5)
            connect(iconBtn[i], SIGNAL(clicked()), this, SLOT(changePenWeight()));
        if (i == 6)
            connect(iconBtn[i], SIGNAL(clicked()), canvas, SLOT(clearScreen()));
        if (i == 7)
            connect(iconBtn[i], SIGNAL(clicked()), this, SLOT(setting()));
    }


    resize(600, 400);
}

void Widget::changePenWeight() {
    QDialog * temp = new QDialog;
    temp -> setFixedSize(220, 100);

    QSlider * penWeightSelect = new QSlider(Qt::Horizontal, temp);
    penWeightSelect -> setMinimum(1);
    penWeightSelect -> setMaximum(50);
    penWeightSelect -> setValue(penWeight);
    penWeightSelect -> setFixedSize(200, 80);
    penWeightSelect -> setGeometry(10, 0, 200, 80);

    QPushButton * accept = new QPushButton(temp);
    accept -> setText("Save");
    accept -> setGeometry((200 - accept -> width()) / 2, 60, accept -> width(), accept -> height());
    connect(accept, SIGNAL(clicked()), temp, SLOT(close()));

    temp -> exec();

    penWeight = penWeightSelect -> value();
    canvas -> setPenWeight(penWeight);
}

void Widget::changePenColor() {
    QColorDialog c;
    c.exec();

    penColor = c.selectedColor();
    iconBtn[4] -> setPalette(penColor);
    canvas -> setPenColor(penColor);
}

void Widget::drawLine() {
    if (clickedStatus[0])
        iconBtn[0] -> setStyleSheet("background-color: #FFF; border: 0px;");
    else if (check())
        iconBtn[0] -> setStyleSheet("background-color: #DDD; border: 1px;");

    clickedStatus[0] = 1 - clickedStatus[0];
    canvas -> drawLineTrigger(clickedStatus[0]);
}

void Widget::drawSquare() {
    if (clickedStatus[1])
        iconBtn[1] -> setStyleSheet("background-color: #FFF; border: 0px;");
    else if (check())
        iconBtn[1] -> setStyleSheet("background-color: #DDD; border: 1px;");

    clickedStatus[1] = 1 - clickedStatus[1];
    canvas -> drawSquareTrigger(clickedStatus[1]);
}

void Widget::drawCircle() {
    if (clickedStatus[2])
        iconBtn[2] -> setStyleSheet("background-color: #FFF; border: 0px;");
    else if (check())
        iconBtn[2] -> setStyleSheet("background-color: #DDD; border: 1px;");

    clickedStatus[2] = 1 - clickedStatus[2];
    canvas -> drawCircleTrigger(clickedStatus[2]);
}

void Widget::drawPolygon() {
    if (clickedStatus[3])
        iconBtn[3] -> setStyleSheet("background-color: #FFF; border: 0px;");
    else if (check())
        iconBtn[3] -> setStyleSheet("background-color: #DDD; border: 1px;");

    clickedStatus[3] = 1 - clickedStatus[3];
    canvas -> drawPolygonTrigger(clickedStatus[3]);
}

Widget::~Widget() {
    delete ui;
}

bool Widget::check() {
    int cnt = 0;
    for (int i = 0; i < Ui::maxn; i ++)
        cnt += clickedStatus[i] == 1;

    return cnt < 1;
}
