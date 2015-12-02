#include "PopBase.h"
#include "UI/Config/Config.h"


CPopBase::CPopBase(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
//    this->setFixedSize(ui_content_width,ui_content_height);
    this->setGeometry(0,0,ui_app_width,ui_app_height);
//    this->setAttribute(Qt::WA_TranslucentBackground);
//    m_labelBackspace.setFixedSize(this->width(),this->height());
//    m_labelBackspace.setAttribute(Qt::WA_TranslucentBackground, false);
////    m_labelBackspace.setStyleSheet("border: 1px solid white;border-radius:10px;background-color:rgb(255,43,61)");
//    m_labelBackspace.setParent(this);

//    m_labelFrame.setAttribute(Qt::WA_TranslucentBackground, false);
//    m_labelBackground.setStyleSheet("border: 1px solid black;border-radius:10px;background-color:rgb(36,43,61)");
////    m_labelBackground.setWindowFlags(Qt::WindowStaysOnBottomHint);

//    connect(&m_labelBackspace, SIGNAL(clicked()), this, SIGNAL(onSpaceCliced()));

    m_timer = new QTimer;
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
