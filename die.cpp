#include "die.h"


Die::Die(QPixmap *image) : currentValue(0), saved(false)
{
    change_pixmap(image);
}


//Changes the pixmap of die
void Die::change_pixmap(QPixmap *image)
{
    setPixmap(*image);
}


//Changes the value and pixmap when thrown
void Die::change_value(int face, QPixmap *image)
{
    currentValue = face;
    change_pixmap(image);
}


void Die::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    toggle_saved();
}


//Toggles between saved and free
void Die::toggle_saved()
{
    if(saved)
    {
        saved = false;
        emit free(scenePos());
    }
    else
    {
        saved = true;
        emit save(scenePos());
    }
}


//Returns saved state
bool Die::is_saved()
{
    return saved;
}


//Returns value of die
int Die::get_value()
{
    return currentValue;
}

