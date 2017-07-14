#include "canvas.h"

Canvas::Canvas(QWidget * parent, QColor _penColor, int _penWeight) : QWidget(parent) {
    setAutoFillBackground(true);
    setPalette(Qt::white);
    setMaximumSize(1366, 768);
    resize(600, 400);

    setStyleSheet(QString("border: 10px solid red;"));

    pix = new QPixmap(size());
    pix -> fill(Qt::white);

    penColor = _penColor, penWeight = _penWeight;
    drawLine = drawSquare = drawCircle = drawPolygon = false;
}

void Canvas::setPenColor(QColor _penColor) {
    penColor = _penColor;
}

void Canvas::setPenWeight(int _penWeight) {
    penWeight = _penWeight;
}

void Canvas::clearScreen() {
    QPixmap * temp = new QPixmap(size());
    temp -> fill(Qt::white);
    pix = temp;
    update();
}

void Canvas::mousePressEvent(QMouseEvent *e) {
    if (drawPolygon){
        grabPen();

        if (e -> button() == Qt::RightButton){
            if (!polygon.empty()) {
                polygon.push_back(polygon[0]);
                for  (int i = 1; i < polygon.size(); i ++)
                    qDebug() << polygon[i] ,
                    painter -> drawLine(polygon[i - 1], polygon[i]);

                polygon.clear();
            }
        }
        else {
            polygon.push_back(e -> pos());
            pen.setWidth(penWeight + 5);
            painter -> setPen(pen);
            painter -> drawPoint(e -> pos());
        }

        painter -> end();
        update();
    }


    start = e -> pos();
}

void Canvas::mouseMoveEvent(QMouseEvent *e) {
    if (drawLine || drawSquare || drawCircle)
        end = e -> pos();
    else if (!drawPolygon) {
        grabPen();
        painter -> drawLine(start, e -> pos());
        painter -> end();
        start = e -> pos();
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent * e) {
    grabPen();

    if (drawLine)
        painter -> drawLine(start, end);
    if (drawSquare) {
        painter -> drawLine(start, QPoint(start.x(), end.y()));
        painter -> drawLine(QPoint(start.x(), end.y()), end);
        painter -> drawLine(end, QPoint(end.x(), start.y()));
        painter -> drawLine(QPoint(end.x(), start.y()), start);
    }
    if (drawCircle) {
        QPoint center((start.x() + end.x()) / 2.0, (start.y() + end.y()) / 2.0);
        int a = abs(start.x() - end.x());
        int b = abs(start.y() - end.y());
        painter -> drawEllipse(center, a, b);
    }

    painter -> end();
    start = end;
    update();
}

void Canvas::grabPen() {
    painter = new QPainter;
    pen.setWidth(penWeight);
    pen.setColor(penColor);
    painter -> begin(pix);
    painter -> setPen(pen);
}

void Canvas::paintEvent(QPaintEvent * e) {
    painter = new QPainter(this);
    painter -> drawPixmap(QPoint(0, 0), * pix);
}

void Canvas::resizeEvent(QResizeEvent * e) {
    if (height() > pix -> height() || width() > pix -> width()) {
        QPixmap * temp = new QPixmap(size());
        temp -> fill(Qt::white);

        QPainter tempPainter(temp);
        tempPainter.drawPixmap(QPoint(0, 0), * pix);
        pix = temp;
    }

    QWidget::resizeEvent(e);
}

void Canvas::drawLineTrigger(int x) {
    drawLine = x;
    setFocusPolicy(Qt::StrongFocus);
}

void Canvas::drawSquareTrigger(int x){
    drawSquare = x;
    setFocusPolicy(Qt::StrongFocus);
}

void Canvas::drawCircleTrigger(int x){
    drawCircle = x;
    setFocusPolicy(Qt::StrongFocus);
}

void Canvas::drawPolygonTrigger(int x){
    drawPolygon = x;
    setFocusPolicy(Qt::StrongFocus);
}
