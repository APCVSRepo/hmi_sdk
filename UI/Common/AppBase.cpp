#include "AppBase.h"
#include "MainMenu.h"
#include <QMouseEvent>

AppBase::AppBase(AppListInterface * pList, QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setGeometry(0,0,parent->width(),parent->height());
//    this->setFixedSize(ConfigSingle::Instance()->getMainWindowW(),ConfigSingle::Instance()->getMainWindowH());
    this->setBkgImage((char *)":/images/mainbkg.png");
    m_pList = pList;
}

AppBase::~AppBase()
{

}

void AppBase::setBkgImage(char *img)
{
    this->setAutoFillBackground(true);    //Widget增加背景图片时，这句一定要。
    QPixmap pixmap(img);
    QPixmap fitpixmap=pixmap.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(fitpixmap));
    this->setPalette(palette);
}


void AppBase::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<"("+QString::number(event->x())+","+QString::number(event->y())+")";

    static int m_i_tmpX=0;
    if(event->buttons()&Qt::LeftButton)
    {
//        qDebug()<<"鼠标左键按下:移动"<<m_i_tmpX<< event->x();

        if(m_i_tmpX != 0 && m_i_tmpX < event->x())
        {
            //moveAllBtnRight();
            //qDebug()<<"鼠标左键按下:右移动";
            m_i_count++;
            if(m_i_count > 10)
            {
                m_i_count = 0;
                m_i_leftRight = 0;
            }
        }
        else if(m_i_tmpX != 0 && m_i_tmpX > event->x())
        {
            //moveAllBtnLeft();
            //qDebug()<<"鼠标左键按下:左移动";
            m_i_count++;
            if(m_i_count > 10)
            {
                m_i_count = 0;
                m_i_leftRight = 1;
            }
        }

        m_i_tmpX = event->x();
    }
}
void AppBase::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_i_leftRight == 0)
    {
//        emit moveBack();
    }
    else if(m_i_leftRight == 1)
    {

    }
    m_i_leftRight = 2;
    m_i_count = 0;
}

void AppBase::execShow()
{

}

void AppBase::receiveJson(Json::Value json)
{

}
