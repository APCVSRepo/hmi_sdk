#include "PopBase.h"
#include "UI/Config/Config.h"

extern Config g_config;
CPopBase::CPopBase(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setFixedSize(g_config.m_i_mainWindowW, g_config.m_i_mainWindowH);
    m_labelBackspace.setFixedSize(g_config.m_i_mainWindowW, g_config.m_i_mainWindowH);
    m_labelBackspace.setAttribute(Qt::WA_TranslucentBackground, false);
//    m_labelBackspace.setStyleSheet("border: 1px solid white;border-radius:10px;background-color:rgb(255,43,61)");
    m_labelBackspace.setParent(this);

    m_labelFrame.setAttribute(Qt::WA_TranslucentBackground, false);
    m_labelBackground.setStyleSheet("border: 1px solid black;border-radius:10px;background-color:rgb(36,43,61)");
//    m_labelBackground.setWindowFlags(Qt::WindowStaysOnBottomHint);

    connect(&m_labelBackspace, SIGNAL(clicked()), this, SIGNAL(onSpaceCliced()));
    m_timer = new QTimer;
}

void CPopBase::execShow(AppDataInterface* pAppInterface)
{

}
