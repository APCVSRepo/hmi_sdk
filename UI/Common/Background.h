#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QLabel>
#include "QMouseEvent"

class Background : public QLabel
{
    Q_OBJECT
public:
    Background();

signals:
    void clicked();
private:
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // BACKGROUND_H
