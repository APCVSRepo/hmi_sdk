#ifndef BASEWIDGHT_H
#define BASEWIDGHT_H

#include <QWidget>

class BaseWidght : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidght(int x,int y,int w,int h,QWidget *parent = 0);
    void setBackGroundImge(char *bkgImg);
    ~BaseWidght();

signals:

public slots:
};

#endif // BASEWIDGHT_H
