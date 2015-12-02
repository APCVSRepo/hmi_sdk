#include <QApplication>
#include <QFontDatabase>
#include <QTextCodec>

#include "UI/Config/Config.h"
#include "UI/UIManager.h"
#include <QDialog>
#include <QTimer>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    ConfigSingle::Instance()->loadResolution(W800_H480);
//    ConfigSingle::Instance()->loadResolution(W1024_H600);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    LOGD("RES width=%d,height=%d\n",QApplication::desktop()->width(),QApplication::desktop()->height());
#ifdef ANDROID
    UIConfig::loadResolution(QApplication::desktop()->width(),QApplication::desktop()->height()-30);
#else
    UIConfig::loadResolution(800,480);
#endif
    QDialog diaStart;
    diaStart.setGeometry(0,0,ui_res_width,ui_res_height);
    diaStart.setStyleSheet("background-image:url(:/images/Screen.png);");
    CUIManager  uiManager;

    //QTimer timer; //定时
    QObject::connect(&uiManager,SIGNAL(finishMainHMI()),&diaStart, SLOT(accept()));
   // QObject::connect(&timer, SIGNAL(timeout()), &diaStart, SLOT(accept()));
   // timer.start(500);//设置时间
    QTimer::singleShot(500,&uiManager,SLOT(onMainHMIStart()));
    diaStart.exec();
   // uiManager.onMainHMIStart();
//    screen.showMainUI();
    uiManager.showMainUI();
    return a.exec();
}
