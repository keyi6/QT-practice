#ifndef WIDGET_H
#define WIDGET_H

#include <QDir>
#include <QIcon>
#include <QSlider>
#include <QString>
#include <QWidget>
#include <QDialog>
#include <QSaveFile>
#include <QPushButton>
#include <QMainWindow>
#include <QColorDialog>

#include "canvas.h"

#include <QDebug>

namespace Ui {
class Widget;
const int maxn = 7;
const QString altInfo[maxn] = {"line", "square", "circle", "polygon", "penColor", "penWeight", "clear"};
}

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void drawLine();
    void drawSquare();
    void drawCircle();
    void drawPolygon();
    void changePenWeight();
    void changePenColor();

private:
    Ui::Widget *ui;

    QColor penColor;
    int penWeight, clickedStatus[Ui::maxn];

    Canvas * canvas;
    QPushButton * iconBtn[Ui::maxn];

    bool check();
};

#endif // WIDGET_H
