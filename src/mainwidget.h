#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QApplication>

class MainWidget : public QGraphicsView
{
    Q_OBJECT

public:
    MainWidget(QGraphicsScene *scene, QWidget *parent = 0);
    ~MainWidget();

    QPixmap getBackgroundPixmap() const;
    void setBackgroundPixmap(const QPixmap &value);

private:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QPixmap background;
};

