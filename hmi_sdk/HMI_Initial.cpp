#include "HMI_Initial.h"
#include <QTimer>
#include "Connect/SDLConnector.h"
#include <QApplication>
#include <main.h>
#include <QDir>
#include <QFontDatabase>

#ifdef linux
#include <unistd.h>
#endif

typedef UIInterface *(*InitFunc)(AppListInterface *);
                typedef void  (*CloseFunc)();
                                typedef int (*addFunc)(int,int);

HMI_Initial::HMI_Initial():QObject(NULL)
{
    m_appList = new AppList;

    std::string strFilePath = GetUILibPath();
    strFilePath += "Gen3UI";
    m_UILib.setFileName(strFilePath.c_str());
    InitFunc Init = (InitFunc)m_UILib.resolve("UILib_Init");
    if (Init){
        m_uiManager = Init(m_appList);
        m_appList->setUIManager(m_uiManager);
    }else{
        LOGE("can't load UILib, %s", strFilePath.data());
    }

    QTimer::singleShot(500,this,SLOT(initApps()));
    //QFontDatabase::addApplicationFont(":/LiberationSerif-Regular.ttf");
}

HMI_Initial::~HMI_Initial()
{
    //delete m_uiManager;
    delete m_appList;
    SDLConnector::Close();

    CloseFunc CloseUI = (CloseFunc)m_UILib.resolve("UILib_Close");
    if (CloseUI){
        CloseUI();
    }
}


void HMI_Initial::initApps()
{
#ifdef SDL_SUPPORT_LIB
    initSDL();
#endif
    //m_uiManager->initAppHMI();
    ToSDL->ConnectToSDL(m_appList);
    emit finishMainHMI();
}

void HMI_Initial::ShowUI()
{
    m_uiManager->onAppShow(ID_MAIN);
    m_uiManager->onAppShow(ID_APPLINK);
}

#ifdef SDL_SUPPORT_LIB
void* HMI_Initial::SDLStartThread(void *arg)
{
#ifdef ANDROID
    if (!FileCopyToConfigdir(":/assets/"))
        return NULL;
    if (!FileCopyToConfigdir(":/config/"))
        return NULL;
#ifdef  TTS_FLY_MSC
    if (!FileCopyToConfigdir(":/msctts/"))
        return NULL;
#endif
#endif
    char  sdlconfig[50] = {0};
    sprintf(sdlconfig,"%s/smartDeviceLink.ini",CONFIG_DIR);
    char* argv[2] = {"smartDeviceLinkCore",sdlconfig};
    sdl_start(2,argv);
    while (true){
        sleep(100);
    }
    return NULL;
}
#ifdef ANDROID
bool HMI_Initial::FileCopyToConfigdir(const char *dir_)
{
    QDir dir(dir_);
    if (!dir.exists()){
        LOGE("%s not exist",dir.dirName().toUtf8().data());
        return false;
    }
    if (!QDir(CONFIG_DIR).exists()){
        QDir dir;
        if (!dir.mkdir(CONFIG_DIR)){
            LOGE("%s create failed",CONFIG_DIR);
            return false;
        }
        else
            LOGE("%s create success",CONFIG_DIR);
    }
    QFileInfoList list = dir.entryInfoList();
    LOGI("total %d files",list.count());
    for (int i = 0; i < list.count(); i++){
        QFileInfo info = list.at(i);

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

void HMI_Initial::initSDL()
{
    pthread_t  sdlthread;
    pthread_create(&sdlthread,NULL,SDLStartThread,NULL);
}
#endif

std::string HMI_Initial::GetUILibPath()
{
    const int iBuffLen = 512;
    char aPathBuff[iBuffLen];
    std::string strResult("");
#if defined(WIN32) || defined(WINCE)
    WCHAR wPathBuff[iBuffLen];
    int iRet = GetModuleFileName(NULL,wPathBuff,iBuffLen);
    //GetCurrentDirectory(iBuffLen,wPathBuff);
    int len = WideCharToMultiByte(CP_ACP,0,wPathBuff,wcslen(wPathBuff),aPathBuff,iBuffLen,NULL,NULL);
    strResult = aPathBuff;
    int pos = strResult.find_last_of('\\',strResult.size()-1);
    strResult.erase(pos,strResult.size()-1);
    strResult += "\\UILib\\";    
#elif defined(__ANDROID__)
    getcwd(aPathBuff,iBuffLen);
    strResult = aPathBuff;
    strResult += "/../lib/";
#elif defined(linux)
    getcwd(aPathBuff,iBuffLen);
    strResult = aPathBuff;
    strResult += "/UILib/";
#endif
    return strResult;
}

