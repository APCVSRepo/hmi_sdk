#include <QApplication>
#include <QFontDatabase>
#include <QTextCodec>

#include "AppData/AppList.h"
#include "UI/UIManager.h"
#include "UI/Config/Config.h"
#include <pthread.h>

#ifdef ANDROID
#include "main.h"
void *SdlStartThread(void *arg);
#endif
int main(int argc, char *argv[])
{
#ifdef ANDROID
    pthread_t  sdlthread;
    pthread_create(&sdlthread,NULL,SdlStartThread,NULL);
#endif
    QApplication a(argc, argv);

//    ConfigSingle::Instance()->loadResolution(W800_H480);
    ConfigSingle::Instance()->loadResolution(W1024_H600);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

    AppList m_appList;
    CUIManager m_ui;

    m_ui.setAppListInterface(&m_appList);
    m_ui.init();

    m_appList.setUIManager(&m_ui);
    m_appList.start();

    return a.exec();
}

#ifdef ANDROID
void *SdlStartThread(void *arg)
{
    char* argv[2]={"smartDeviceLinkCore","/data/data/org.qtproject.example.AppLinkDevice/files/smartDeviceLink.ini"};
    sdl_start(2,argv);
    while(true){
        sleep(5);
    }
    return NULL;
}
#endif
