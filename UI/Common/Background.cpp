#include "Background.h"

Background::Background()
{
}

void Background::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        emit clicked();
    }
}

