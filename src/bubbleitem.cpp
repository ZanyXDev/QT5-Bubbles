#include "bubbleitem.h"

#define _USE_MATH_DEFINES


static double RADS_IN_DEG = M_PI / 180.0;

const qreal  BubbleItem::RADIUS = 90; // радиус шаров в пикселях
const qreal  BubbleItem::COLORIZER_STRENGTH = 0.27; // чем больше этот параметр - тем ярче цвет пузырей

BubbleItem::BubbleItem()
    : speed( QRandomGenerator::global()->bounded(33) )
    , can_bounce(true)
{
    setRotation(QRandomGenerator::global()->bounded(360));
    setGraphicsEffect( rndEffect() );
}

QRectF BubbleItem::boundingRect() const
{
    return QRectF(-RADIUS, -RADIUS, 2 * RADIUS, 2 * RADIUS);
}

QPainterPath BubbleItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void BubbleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->save();
    painter->rotate( -rotation() );

    painter->drawPixmap(boundingRect(), m_pixmap, m_pixmap.rect());
    // Пример дополнительной декорации
    //painter->setBrush(Qt::Dense7Pattern);
    //painter->drawEllipse(boundingRect());

    painter->restore();
}

void BubbleItem::setBubblePixmap(const QPixmap &pixmap)
{
    m_pixmap =pixmap;
}

void BubbleItem::advance(int step)
{
    if (step == 0){
        return;
    }

    QList<QGraphicsItem*> collisions = scene()->collidingItems(this);

    if ( can_bounce ){
        // Обрабатаем столновения с другими шарами
        if ( !collisions.isEmpty() )
        {
            for (auto& collisionGraphicsItem : collisions) {
                if (auto item = qgraphicsitem_cast<BubbleItem*>(collisionGraphicsItem)) {
                    // swap rotation
                    qreal other_rot = item->rotation();
                    item->setRotation(this->rotation());
                    setRotation(other_rot);

                    // change color effect

                    item->setGraphicsEffect(  rndEffect() );
                    setGraphicsEffect( rndEffect() );
                    //swap speed
                    std::swap( this->speed, item->speed );
                }
            }
            // Предотвращаем "склеивание" шаров
            can_bounce = false;
        }
    }else{
        if ( collisions.isEmpty() ){
            can_bounce = true;
            if (this->speed == 0) {
                this->speed = QRandomGenerator::global()->bounded(33);
            }
        }
    }


    // Проверим столкновения с краями экрана
    const QRectF scene_rect = scene()->sceneRect();
    const qreal x = QGraphicsItem::pos().x();
    const qreal y = QGraphicsItem::pos().y();

    QPointF vec( std::cos(RADS_IN_DEG * rotation()),
                 std::sin(RADS_IN_DEG * rotation()));

    if (x - RADIUS < scene_rect.left())
        vec.setX( qAbs(vec.x()) );

    if (x + RADIUS > scene_rect.right())
        vec.setX( -qAbs(vec.x()) );

    if (y - RADIUS < scene_rect.top())
        vec.setY( qAbs(vec.y()) );

    if (y + RADIUS > scene_rect.bottom())
        vec.setY( -qAbs(vec.y()) );

    qreal new_angle = std::acos(vec.x()) / RADS_IN_DEG;
    if ( vec.y() < 0 )
        new_angle = 360.0 - new_angle;

    setRotation(new_angle);

    // Сделаем шаг
    setPos( mapToScene(speed, 0) );
}

QGraphicsColorizeEffect *BubbleItem::rndEffect() const
{
    QColor color( QRandomGenerator::global()->bounded(256),
                  QRandomGenerator::global()->bounded(256),
                  QRandomGenerator::global()->bounded(256) );
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
    effect->setColor(color);
    effect->setStrength(COLORIZER_STRENGTH);
    return effect;
}
