#include "AppManager.h"
#include <QTimer>
#include <QDesktopWidget>
#include "Connect/SDLConnector.h"
#include <QApplication>
#include <main.h>
#include <QDir>

AppManager::AppManager():QObject(NULL)
{
#ifdef ANDROID
    UIConfig::loadResolution(QApplication::desktop()->width(),QApplication::desktop()->height()-30);
#else
    UIConfig::loadResolution(800,480);
#endif
    m_appList = new AppList;
    m_uiManager = new CUIManager(m_appList);
    m_appList->setUIManager(m_uiManager);
    QTimer::singleShot(500,this,SLOT(initApps()));
}

AppManager::~AppManager()
{
    delete m_uiManager;
    delete m_appList;
}



void AppManager::initApps()
{
#ifdef SDL_SUPPORT_LIB
    initSDL();
#endif
    m_uiManager->initAppHMI();
    SDLConnector::getSDLConnectore()->ConnectToSDL(m_appList);
    emit finishMainHMI();
}

void AppManager::ShowUI()
{
    m_uiManager->showMainUI();
}

#ifdef SDL_SUPPORT_LIB
void* AppManager::SDLStartThread(void *arg)
{
#ifdef ANDROID
    if(!FileCopyToConfigdir(":/assets/"))
        return NULL;
    if(!FileCopyToConfigdir(":/config/"))
        return NULL;
#ifdef  TTS_FLY_MSC
    if(!FileCopyToConfigdir(":/msctts/"))
        return NULL;
#endif
#endif
    char  sdlconfig[50]={0};
    sprintf(sdlconfig,"%s/smartDeviceLink.ini",CONFIG_DIR);
    char* argv[2]={"smartDeviceLinkCore",sdlconfig};
    sdl_start(2,argv);
    while(true){
        sleep(100);
    }
    return NULL;
}
#ifdef ANDROID
bool AppManager::FileCopyToConfigdir(const char *dir_)
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

void AppManager::initSDL()
{
    pthread_t  sdlthread;
    pthread_create(&sdlthread,NULL,SDLStartThread,NULL);
}
#endif

