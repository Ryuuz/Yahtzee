#ifndef DIE_H
#define DIE_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>

class Die : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    public:
        Die(QPixmap *image);
        void change_pixmap(QPixmap *image);
        void change_value(int face, QPixmap *image);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void toggle_saved();
        bool is_saved();
        int get_value();

    signals:
        void save(QPointF mousePos);
        void free(QPointF mousePos);

    private:
        int currentValue;
        bool saved;
};

#endif // DIE_H
