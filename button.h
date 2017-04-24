#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>

class Button : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    public:
        Button(QPixmap *image);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);

    signals:
        void clicked();
};

#endif // BUTTON_H
