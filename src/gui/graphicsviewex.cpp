#include "graphicsviewex.h"

GraphicsViewEx::GraphicsViewEx(QWidget *parent)
    : QGraphicsView(parent){
    setCursor(Qt::PointingHandCursor);
}

void GraphicsViewEx::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QGraphicsView::mousePressEvent(event);
}

