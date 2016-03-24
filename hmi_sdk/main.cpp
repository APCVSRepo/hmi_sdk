#include <QApplication>
#include <QDesktopWidget>
#include <QTextCodec>

//#include "UI/Config/Config.h"
#include <QDialog>
#include "HMI_Initial.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
#endif

    HMI_Initial appManager;
    QDialog diaStart;
#ifdef ANDROID
    diaStart.setGeometry(0,0,QApplication::desktop()->width(),QApplication::desktop()->height()-30);
#else
    diaStart.setGeometry(0,0,800,480);
#endif
    diaStart.setStyleSheet("border-image:url(:/images/Screen.png);");
    QObject::connect(&appManager,SIGNAL(finishMainHMI()),&diaStart, SLOT(accept()));
    diaStart.exec();
    appManager.ShowUI();

    return a.exec();
}
