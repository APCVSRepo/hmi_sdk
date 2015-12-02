#include "SplashScreen.h"
#include <QImage>
#include <QPainter>
#include "Config/Config.h"
#include <pthread.h>
#include <QThread>

#ifdef ANDROID
#include "android/log.h"
#include <unistd.h>
#endif

#include<sys/stat.h>
#include <QDir>
#ifdef SDL_SUPPORT_LIB
#include "main.h"
#endif
SplashScreen::SplashScreen(QWidget *parent) : QWidget(parent)
{
    m_screen_w=ui_res_width;
    m_screen_h=ui_res_height;
    if(m_screen_h<=0||m_screen_w<=0)
    {
        m_screen_w=1024;
        m_screen_h=600;
    }
    this->setGeometry(0,0,m_screen_w,m_screen_h);
    //this->show();
    //update();
   // connect(this,SIGNAL(startInitUI()),SLOT(onUIWait()),Qt::QueuedConnection);
}


SplashScreen::~SplashScreen()
{

}

void SplashScreen::showMainUI()
{
    m_ui->showMainUI();
}

void SplashScreen::onUIInit()
{
    pthread_t  sdlthread;
    pthread_create(&sdlthread,NULL,SplashScreen::SDLStartThread,NULL);
    m_ui=new CUIManager;
    manager=new DataManager(m_ui);
    manager->start();
    emit finishUIInit();
}



void SplashScreen::paintEvent(QPaintEvent *e)
{
    QImage img(":/images/Screen.png","png");
    QPainter painter(this);
    painter.drawImage(QRect(0,0,m_screen_w,m_screen_h),img);
}

