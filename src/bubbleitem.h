#pragma once

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsColorizeEffect>
#include <QPainter>
#include <QGraphicsScene>
#include <QRandomGenerator>

#include <QDebug>

#include <cmath>

class BubbleItem : public QGraphicsItem
{
public:
    static const qreal  RADIUS;
    static const qreal  COLORIZER_STRENGTH;

public:
    BubbleItem();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setBubblePixmap(const QPixmap &pixmap);

    enum { Type = UserType + 1 };

    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }
protected:
    void advance(int step) override;

private:
    qreal speed;
    bool can_bounce;
    QPixmap     m_pixmap;

    QGraphicsColorizeEffect *rndEffect() const;
};
