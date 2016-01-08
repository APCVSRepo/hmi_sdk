#include "UIManager.h"
#include "AppData/AppList.h"
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
#include "AudioTrans/MspVRAudio.h"
#endif

CUIManager::CUIManager(AppListInterface * pList, QWidget *parent) :
    QWidget(parent)
{
    m_pAppList = pList;
}

CUIManager::~CUIManager()
{
    for(int i = 0; i < ID_UI_MAX; i++)
        if(m_vUIWidgets[i])
            delete m_vUIWidgets[i];
#ifdef SDL_SUPPORT_LIB
    delete m_MspVR;
#endif
}

void CUIManager::initAppHMI()
{
    MainMenu * pMain = new MainMenu(m_pAppList);
    QWidget* pParent = pMain->CenterWidget();
    m_vUIWidgets[ID_MAIN]= pMain;
    m_vUIWidgets[ID_APPLINK]=new AppLink(m_pAppList, pParent);
    m_vUIWidgets[ID_ALERT]=new CAlertUI(m_pAppList);
    m_vUIWidgets[ID_AUDIOPASSTHRU]=new CAudioPassThru(m_pAppList);
    m_vUIWidgets[ID_CHOICESETVR]=new CChoicesetVR(m_pAppList);
    m_vUIWidgets[ID_CHOICESET]=new Choiceset(m_pAppList, pParent);
    m_vUIWidgets[ID_COMMAND]=new Command(m_pAppList, pParent);
    m_vUIWidgets[ID_SCROLLMSG]=new CScrollMsg(m_pAppList);
    m_vUIWidgets[ID_SHOW]=new Show(m_pAppList, pParent);
    m_vUIWidgets[ID_NOTIFY]=new Notify;
    m_vUIWidgets[ID_SLIDER]=new Slider(m_pAppList);
    m_vUIWidgets[ID_MEDIACLOCK] = NULL;

    for(int i = 0; i < ID_UI_MAX; i++)
        if(m_vUIWidgets[i])
            m_vUIWidgets[i]->hide();

    m_iCurUI = ID_MAIN;

#ifdef SDL_SUPPORT_LIB
    m_MspVR = new msp_vr_audio;
#endif
//
    connect(this,SIGNAL(onAppShowSignal(int)),this,SLOT(AppShowSlot(int)));
    connect(this,SIGNAL(onCloseUISignal()),this,SLOT(CloseUISlot()));

    connect(this,SIGNAL(onVideoStartSignal()),this,SLOT(onVideoStartSlots()));
    connect(this,SIGNAL(onVideoStopSignal()),this,SLOT(onVideoStopSlots()));

    //emit finishMainHMI();
}

//show app
void CUIManager::onAppShow(int type)
{
    emit onAppShowSignal(type);
}

void CUIManager::onVideoStreamStart()
{
    fflush(stdout);
    emit onVideoStartSignal();
}

void CUIManager::onVideoStartSlots()
{
    fflush(stdout);
    std::string str_url = m_pAppList->getActiveApp()->getUrlString();
    //_D("%s\n",str_url.data());
    ((MainMenu *)m_vUIWidgets[ID_MAIN])->StartVideoStream(str_url.c_str());
}

void CUIManager::onVideoStreamStop()
{
    emit onVideoStopSignal();
}

void CUIManager::onVideoStopSlots()
{
    ((MainMenu *)m_vUIWidgets[ID_MAIN])->StopVideoStream();
}

void CUIManager::AppShowSlot(int type)
{
    if(ID_MEDIACLOCK == type)
    {
        if(ID_SHOW == m_iCurUI)
        {
            Show * pShow = (Show *)m_vUIWidgets[ID_SHOW];
            pShow->UpdateMediaColckTimer();
        }
    }
    else
    {
        if(m_iCurUI != ID_MAIN)
            m_vUIWidgets[m_iCurUI]->hide();
        m_iCurUI = type;
        m_vUIWidgets[m_iCurUI]->show();
    }
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
    bool ret = ts.StartVoiceThread(strText);
    /*
    while(ts.isSpeaking())
        waitMSec(100);
        */

    switch(VRID)
    {
    case ID_DEFAULT:
        if(ret)
            m_pAppList->getActiveApp()->OnTTSSpeek(0);
        else
            m_pAppList->getActiveApp()->OnTTSSpeek(5);
        break;
    case ID_CANCEL:
        m_pAppList->getActiveApp()->OnPerformAudioPassThru(PERFORMAUDIOPASSTHRU_CANCEL);
        break;
    case ID_HELP:
        m_pAppList->getActiveApp()->OnPerformAudioPassThru(PERFORMAUDIOPASSTHRU_DONE);
        break;
    case ID_EXIT:
        m_pAppList->getActiveApp()->OnPerformAudioPassThru(PERFORMAUDIOPASSTHRU_DONE);
        m_pAppList->OnAppExit();
        break;
    case ID_SWITCHAPP:
        m_pAppList->getActiveApp()->OnPerformAudioPassThru(PERFORMAUDIOPASSTHRU_DONE);
        break;
    default:
        m_pAppList->getActiveApp()->OnPerformAudioPassThru(PERFORMAUDIOPASSTHRU_CANCEL);
        break;
    }
}
