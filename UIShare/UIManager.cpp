﻿#include "UIManager.h"
#include <QApplication>
#include <QDesktopWidget>
//#include "AppData/AppList.h"
//#include <pthread.h>
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

#include "UI/Alert/AlertUI.h"
#include "VideoStream/VideoStream.h"

CUIManager::CUIManager(AppListInterface * pList, QWidget *parent) :
    QWidget(parent)
{
    for(int i = 0; i < ID_UI_MAX; i++)
    {
        m_vUIWidgets[i] = nullptr;
    }
    m_pList = pList;
}

CUIManager::~CUIManager()
{
    for(int i = 0; i < ID_UI_MAX; i++)
    {
        if(m_vUIWidgets[i])
        {
            delete m_vUIWidgets[i];
            m_vUIWidgets[i] = nullptr;
        }
    }


#ifdef SDL_SUPPORT_LIB
    delete m_MspVR;
#endif
}

void CUIManager::initAppHMI()
{
#ifdef ANDROID
    UIConfig::loadResolution(QApplication::desktop()->width(),QApplication::desktop()->height()-30);
#else
    UIConfig::loadResolution(800,480);
#endif

    MainMenu * pMain = new MainMenu(m_pList);
    QWidget* pParent = pMain->CenterWidget();
    pMain->show();
    m_vUIWidgets[ID_MAIN] = pMain;
    m_vUIWidgets[ID_APPLINK] = new AppLinkMenu(m_pList);
    m_vUIWidgets[ID_ALERT]=new CAlertUI(m_pList, pParent);
    m_vUIWidgets[ID_AUDIOPASSTHRU]=new CAudioPassThru(m_pList, pParent);
    m_vUIWidgets[ID_CHOICESETVR]=new CChoicesetVR(m_pList, pParent);
    m_vUIWidgets[ID_CHOICESET]=new Choiceset(m_pList, pParent);
    m_vUIWidgets[ID_COMMAND]=new Command(m_pList, pParent);
    m_vUIWidgets[ID_SCROLLMSG]=new CScrollMsg(m_pList, pParent);
    m_vUIWidgets[ID_SHOW] = new Show(m_pList, pParent);
    m_vUIWidgets[ID_NOTIFY]=new Notify(pParent);
    m_vUIWidgets[ID_SLIDER]=new Slider(m_pList, pParent);
    m_vUIWidgets[ID_MEDIACLOCK] = NULL;
    m_vUIWidgets[ID_VIDEOSTREAM] = new VideoStream(m_pList,pMain);

    for(int i = 0; i < ID_UI_MAX; i++)
    {
        if(m_vUIWidgets[i] != nullptr)
        {
            m_vUIWidgets[i]->hide();
        }
    }

    m_iCurUI = ID_MAIN;

#ifdef SDL_SUPPORT_LIB
    m_MspVR = new msp_vr_audio;
#endif
//
    connect(this,SIGNAL(onAppShowSignal(int)),this,SLOT(AppShowSlot(int)));
    connect(this,SIGNAL(onVideoStartSignal()),this,SLOT(onVideoStartSlots()));
    connect(this,SIGNAL(onVideoStopSignal()),this,SLOT(onVideoStopSlots()));

    //emit finishMainHMI();
}

void CUIManager::onAppActive()
{
    QString qs = AppControl->getAppName().c_str();
    ((MainMenu *)m_vUIWidgets[ID_MAIN])->SetTitle(qs);
    //((MainWidget *)m_vUIWidgets[ID_MAIN])->SetAppName(qs);
}

void CUIManager::onAppStop()
{

}

//show app
void CUIManager::onAppShow(int type)
{
    if((type >= 0) && (type < ID_UI_MAX))
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
    std::string str_url = AppControl->getUrlString();
    //_D("%s\n",str_url.data());
    VideoStream *pVideoStream = ((VideoStream *)m_vUIWidgets[ID_VIDEOSTREAM]);//->StartVideoStream(str_url.c_str());
    pVideoStream->setUrl(str_url.c_str());
    pVideoStream->startStream();
}

void CUIManager::onVideoStreamStop()
{
    emit onVideoStopSignal();
}

void CUIManager::onVideoStopSlots()
{
    ((VideoStream *)m_vUIWidgets[ID_VIDEOSTREAM])->stopStream();
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
        {
            m_vUIWidgets[m_iCurUI]->hide();
        }
        //m_vUIWidgets[m_iCurUI]->hide();
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
    bool ret = ts.speak(strText.c_str());
    /*
    while(ts.isSpeaking())
        waitMSec(100);
        */

    switch(VRID)
    {
    case ID_DEFAULT:
        if(ret)
            AppControl->OnTTSSpeek(0);
        else
            AppControl->OnTTSSpeek(5);
        break;
    case ID_CANCEL:
        AppControl->OnPerformAudioPassThru(PERFORMAUDIOPASSTHRU_CANCEL);
        break;
    case ID_HELP:
        AppControl->OnPerformAudioPassThru(PERFORMAUDIOPASSTHRU_DONE);
        break;
    case ID_EXIT:
        AppControl->OnPerformAudioPassThru(PERFORMAUDIOPASSTHRU_DONE);
        m_pList->OnAppExit();
        break;
    case ID_SWITCHAPP:
        AppControl->OnPerformAudioPassThru(PERFORMAUDIOPASSTHRU_DONE);
        break;
    default:
        AppControl->OnPerformAudioPassThru(PERFORMAUDIOPASSTHRU_CANCEL);
        break;
    }
}

void CUIManager::OnEndAudioPassThru()
{
    ((CAudioPassThru *)m_vUIWidgets[ID_AUDIOPASSTHRU])->onButtonClickedSlots(PERFORMAUDIOPASSTHRU_CANCEL);
}
