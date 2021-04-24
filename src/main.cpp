#include <QApplication>
#include <QTime>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QTimer>
#include <QScreen>
#include <QDebug>

#include "mainwidget.h"
#include "bubbleitem.h"


const int BUBBLES_AMOUNT = 8; // Количество пузырей на экране
const int EXPECTED_FPS   = 30; // Ожидаемое количество кадров в секунду

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    qDebug() << QString("branch: %1, version: %2, built_at: %3").arg(GIT_BRANCH,GIT_HASH,BUILD_TIMESTAMP);

    // Получаем скриншот
    QScreen *screen = QApplication::primaryScreen();
    if (!screen)
        return -1;
    QPixmap screenshot = screen->grabWindow(0);

    // QGraphicsScene - контейнер для создаваемых нами
    // объектов класса Bubble
    QGraphicsScene scene;
    scene.setSceneRect( screenshot.rect() );
    //scene.setSceneRect( 0,0,800,600 );
    // Наполняем сцену непересекающимися элементами
    {
        const int left   = scene.sceneRect().left()   + BubbleItem::RADIUS;
        const int top    = scene.sceneRect().top()    + BubbleItem::RADIUS;
        const int right  = scene.sceneRect().right()  - BubbleItem::RADIUS;
        const int bottom = scene.sceneRect().bottom() - BubbleItem::RADIUS;

        for (int i = 0; i < BUBBLES_AMOUNT; ++i)
        {
            BubbleItem *bubble = new BubbleItem();
            bubble->setBubblePixmap(QPixmap(":/res/images/bubble.png"));
            scene.addItem(bubble);

            // Будем давать пузырю случайные координаты до тех пор
            // пока он не прекратит пересекаться с другими пузырями
            do
            {
                bubble->setPos(
                            QRandomGenerator::global()->bounded(left, right),
                            QRandomGenerator::global()->bounded(top, bottom)
                            );
                //qDebug() << "bubble->pos():"<< bubble->pos();
            }
            while ( !scene.collidingItems(bubble).isEmpty() );
        }
    }

    // MainWidget - главный и единственный виджет этой программы,
    // он непосредственно рисует на экране элементы сцены, а также
    // обрабатывает нажатие клавиш и завершает выполнение программы при
    // нажатии клавиши ESC
    MainWidget view(&scene);
    view.setBackgroundPixmap(screenshot);
    view.setRenderHint(QPainter::Antialiasing);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //view.resize(800,600);
    //view.show();
    view.showFullScreen();

    // Используем QTimer для анимации движения пузырей. Сцена будет принудительно
    // обновляться EXPECTED_FPS раз в секунду
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
    timer.start(1000 / EXPECTED_FPS);

    return app.exec();
}
