#ifndef GRAPHICSVIEWEX_H
#define GRAPHICSVIEWEX_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QObject>
#include <QWidget>

class GraphicsViewEx : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsViewEx(QWidget *parent = nullptr);
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // GRAPHICSVIEWEX_H
