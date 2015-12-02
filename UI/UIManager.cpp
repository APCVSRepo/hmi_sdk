#include "UIManager.h"
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

CUIManager::CUIManager(QWidget *parent) :
    QWidget(parent)
{

}

CUIManager::~CUIManager()
{

}

void CUIManager::onMainHMIStart()
{
#ifdef SDL_SUPPORT_LIB
    this->initSDL();
#endif
    this->initAppHMI();
    emit finishMainHMI();
}

void CUIManager::initSDL()
{
#ifdef SDL_SUPPORT_LIB
    pthread_t  sdlthread;
    pthread_create(&sdlthread,NULL,CUIManager::SDLStartThread,NULL);
#endif
}

void CUIManager::initAppHMI()
{
    m_MainMenu=new MainMenue;
    m_MainMenu->InserWidget(ID_APPLINK,new AppLink(m_MainMenu->CenterWidght()));//
    m_MainMenu->InserWidget(ID_ALERT,new CAlertUI);
    m_MainMenu->InserWidget(ID_AUDIOPASSTHRU,new CAudioPassThru);
    m_MainMenu->InserWidget(ID_CHOICESETVR,new CChoicesetVR);
    m_MainMenu->InserWidget(ID_CHOICESET,new Choiceset(m_MainMenu->CenterWidght()));//
    m_MainMenu->InserWidget(ID_COMMAND,new Command(m_MainMenu->CenterWidght()));//
    m_MainMenu->InserWidget(ID_SCROLLMSG,new CScrollMsg);
    m_MainMenu->InserWidget(ID_SHOW,new Show(m_MainMenu->CenterWidght()));//
    m_MainMenu->InserWidget(ID_NOTIFY,new Notify);
    m_MainMenu->InserWidget(ID_SLIDER,new Slider);
//
    manager=new DataManager(this);
    manager->start();
    connect(this,SIGNAL(onAppShowSignal(int)),this,SLOT(AppShowSlot(int)));
    connect(this,SIGNAL(onAppCloseSignal()),this,SLOT(AppCloseSlot()));
    connect(this,SIGNAL(onAppRefreshSignal()),this,SLOT(AppRefreshSlot()));

    connect(this,SIGNAL(onTestStartSignal()),this,SLOT(onTestStartSlots()));
    connect(this,SIGNAL(onTestStopSignal()),this,SLOT(onTestStopSlots()));
}

void CUIManager::showMainUI()
{
    m_MainMenu->SetCurWidget(ID_APPLINK);
    m_MainMenu->show();
}


#ifdef SDL_SUPPORT_LIB
void* CUIManager::SDLStartThread(void *arg)
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

bool CUIManager::FileCopyToConfigdir(const char *dir_)
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

void CUIManager::SDLKillThread()
{

}
#endif
//show app
void CUIManager::onAppShow(int type)
{
    emit onAppShowSignal(type);
}
void CUIManager::onTestVideoStreamStart()
{
    printf("emit onTestVideoStreamStart");
    fflush(stdout);
    emit onTestStartSignal();
}
void CUIManager::onTestStartSlots()
{
    printf("onTestStartSlots");
    fflush(stdout);
    std::string str_url = DataManager::DataInterface()->getUrlString();
    //_D("%s\n",str_url.data());
#ifdef VIDEO_TEST
    m_videoStreamWidget.setUrl(str_url.data());
    m_videoStreamWidget.startStream();
#endif
}
void CUIManager::onTestVideoStreamStop()
{
    emit onTestStopSignal();
}
void CUIManager::onTestStopSlots()
{
#ifdef VIDEO_TEST
    m_videoStreamWidget.stopStream();
    m_videoStreamWidget.hide();
#endif
}

void CUIManager::onAppActivated(AppDataInterface* pInterface)
{
//    emit onAppActivatedSignal(pInterface);

}

void CUIManager::onAppClose()
{
    emit onAppCloseSignal();
}

void CUIManager::onAppRefresh()
{
    emit onAppRefresh();
}

void CUIManager::AppShowSlot(int type)
{
    m_MainMenu->SetCurWidget(type);
}

void CUIManager::AppCloseSlot()
{

}

void CUIManager::AppRefreshSlot()
{

}

void CUIManager::setMediaColckTimer(Json::Value jsonObj)
{
    //m_pShow->setMediaColckTimer(jsonObj);
    m_MainMenu->ReceiveJson(ID_SHOW,jsonObj);
}

#include <QCoreApplication>
void CUIManager::waitMSec(int ms)
{
    QTime t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void CUIManager::tsSpeak(int VRID, std::string strText)
{
    bool ret=ts.speak(strText.data());
    while(ts.isSpeaking())
        waitMSec(100);

    switch(VRID)
    {
    case ID_DEFAULT:
        if(ret)
         DataManager::DataInterface()->OnTTSSpeek(DataManager::AppId(),0);
        else
          DataManager::DataInterface()->OnTTSSpeek(DataManager::AppId(),5);
        break;
    case ID_CANCEL:
        DataManager::DataInterface()->OnPerformAudioPassThru(DataManager::AppId(), 0, PERFORMAUDIOPASSTHRU_CANCEL);
        DataManager::DataInterface()->OnVRCancelRecord();
        //m_pAudioPassThru->hide();
        m_MainMenu->ExitWidget(ID_AUDIOPASSTHRU);
        break;
    case ID_HELP:
        DataManager::DataInterface()->OnPerformAudioPassThru(DataManager::AppId(), 0, PERFORMAUDIOPASSTHRU_DONE);
        DataManager::DataInterface()->OnVRCancelRecord();
        m_MainMenu->ExitWidget(ID_AUDIOPASSTHRU);
        break;
    case ID_EXIT:
        DataManager::DataInterface()->OnPerformAudioPassThru(DataManager::AppId(), 0, PERFORMAUDIOPASSTHRU_DONE);
        DataManager::DataInterface()->OnVRCancelRecord();
        m_MainMenu->ExitWidget(ID_AUDIOPASSTHRU);
        DataManager::ListInterface()->OnApplicationOut(DataManager::AppId());
        this->onAppShow(ID_APPLINK);
        break;
    case ID_SWITCHAPP:
        DataManager::DataInterface()->OnPerformAudioPassThru(DataManager::AppId(), 0, PERFORMAUDIOPASSTHRU_DONE);
        DataManager::DataInterface()->OnVRCancelRecord();
        m_MainMenu->ExitWidget(ID_AUDIOPASSTHRU);
        break;
    default:
        m_MainMenu->ExitWidget(ID_AUDIOPASSTHRU);
        break;
    }
}

void CUIManager::switchNewApp(int newAppID)
{
    DataManager::switchNewApp(newAppID);
    m_MainMenu->SetCurWidget(ID_SHOW);
}
