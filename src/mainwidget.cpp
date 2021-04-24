#include "mainwidget.h"

#include <QPainter>

MainWidget::MainWidget(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setStyleSheet("QGraphicsView { border-style: none; }");
}

MainWidget::~MainWidget()
{
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        qApp->quit();
        break;

    default:
        break;
    }
    QGraphicsView::keyPressEvent(event);

}

void MainWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    painter.setPen(Qt::NoPen);

    if ( !background.isNull() ){
        painter.drawPixmap(this->rect(), background);
    }

    QGraphicsView::paintEvent(event);
}

QPixmap MainWidget::getBackgroundPixmap() const
{
    return background;
}

void MainWidget::setBackgroundPixmap(const QPixmap &value)
{
    background = value;
}
