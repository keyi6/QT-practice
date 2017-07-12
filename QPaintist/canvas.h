#ifndef CANVAS_H
#define CANVAS_H

#include <QPen>
#include <QPoint>
#include <QColor>
#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QPainter>
#include <QPalette>
#include <QPaintEvent>
#include <QMouseEvent>

#include <QDebug>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr, QColor _penColor = Qt::black, int _penWeight = 1);
    void mousePressEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void paintEvent(QPaintEvent * e);
    void resizeEvent(QResizeEvent * e);

    void drawLineTrigger(int x);
    void drawSquareTrigger(int x);
    void drawCircleTrigger(int x);
    void drawPolygonTrigger(int x);

private:
    int penWeight;
    QPixmap * pix;
    QColor penColor;
    QPoint start, end;
    QVector<QPoint> polygon;

    QPen pen;
    QPainter * painter;

    bool drawLine, drawSquare, drawCircle, drawPolygon;

    void grabPen();

public slots:
    void setPenColor(QColor _penColor);
    void setPenWeight(int _penWeight);
    void clearScreen();
};

#endif // CANVAS_H
