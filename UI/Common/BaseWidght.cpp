#include "BaseWidght.h"

BaseWidght::BaseWidght(int x,int y,int w,int h,QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setGeometry(x,y,w,h);
}

void BaseWidght::setBackGroundImge(char *bkgImg)
{
    this->setAutoFillBackground(true);    //Widget增加背景图片时，这句一定要。
    QPixmap pixmap(bkgImg);
    QPixmap fitpixmap=pixmap.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(fitpixmap));
    this->setPalette(palette);
   // this->setStyleSheet(QString("border-images:url(%1)").arg(bkgImg));
}

BaseWidght::~BaseWidght()
{

}

