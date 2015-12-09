#include <QApplication>
#include <QFontDatabase>
#include <QTextCodec>

#include "UI/Config/Config.h"
#include "UI/UIManager.h"
#include <QDialog>
#include <QTimer>
#include <QDesktopWidget>
#include "Connect/SDLConnector.h"
#include "AppData/AppList.h"

#ifdef SDL_SUPPORT_LIB
void* SDLStartThread(void *arg)
{
    if(!FileCopyToConfigdir(":/assets/"))
        return NULL;
    if(!FileCopyToConfigdir(":/config/"))
        return NULL;

    char  sdlconfig[50]={0};
    sprintf(sdlconfig,"%s/smartDeviceLink.ini",CONFIG_DIR);
    char* argv[2]={"smartDeviceLinkCore",sdlconfig};
    sdl_start(2,argv);
    while(true){
        sleep(100);
    }
    return NULL;
}

bool FileCopyToConfigdir(const char *dir_)
{
    QDir  dir(dir_);
    if(!dir.exists()){
        LOGE("%s not exist",dir.dirName().toUtf8().data());
        return false;
    }
    if(!QDir(CONFIG_DIR).exists()){
        QDir dir;
        if(!dir.mkdir(CONFIG_DIR)){
            LOGE("%s create failed",CONFIG_DIR);
            return false;
        }
        else
            LOGE("%s create success",CONFIG_DIR);
    }
    QFileInfoList list=dir.entryInfoList();
    LOGI("total %d files",list.count());
    for(int i=0;i<list.count();i++){
        QFileInfo info=list.at(i);

        QFile file_in(info.filePath());
        file_in.open(QFile::ReadOnly);
        QFile file_out(QString(CONFIG_DIR)+"/"+info.fileName());
        file_out.open(QFile::ReadWrite);
        file_out.write(file_in.readAll());
        file_out.close();
        file_in.close();
        LOGI("%s copy to %s",file_in.fileName().toUtf8().data(),file_out.fileName().toUtf8().data());
    }
    return true;
}

#endif

void initSDL()
{
#ifdef SDL_SUPPORT_LIB
    pthread_t  sdlthread;
    pthread_create(&sdlthread,NULL,SDLStartThread,NULL);
#endif
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif


    QDialog diaStart;
    diaStart.setGeometry(0,0,ui_res_width,ui_res_height);
    diaStart.setStyleSheet("background-image:url(:/images/Screen.png);");

#ifdef SDL_SUPPORT_LIB
    initSDL();
#endif

    AppList AppList;
    CUIManager  uiManager(&AppList);
    SDLConnector::getSDLConnectore()->ConnectToSDL(&AppList);

    //QTimer timer; //定时
    QObject::connect(&uiManager,SIGNAL(finishMainHMI()),&diaStart, SLOT(accept()));
    QTimer::singleShot(500,&uiManager,SLOT(initAppHMI()));
    diaStart.exec();


    uiManager.showMainUI();
    return a.exec();
}
