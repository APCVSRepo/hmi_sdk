#include "PopBase.h"
#include "UI/Config/Config.h"


CPopBase::CPopBase(AppListInterface * pList, QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
//    this->setFixedSize(ui_content_width,ui_content_height);
    this->setGeometry(0,0,ui_app_width,ui_app_height);
    this->setBkgImage(":images/mainbkg.png");
    m_timer = new QTimer;
    m_pList = pList;
}
void CPopBase::setBkgImage(const char *img)
{
    this->setAutoFillBackground(true);    //Widget增加背景图片时，这句一定要。
    QPixmap pixmap(img);
    QPixmap fitpixmap=pixmap.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(fitpixmap));
    this->setPalette(palette);
}
void CPopBase::execShow()
{

}

void CPopBase::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        emit onSpaceCliced();
    }
}
