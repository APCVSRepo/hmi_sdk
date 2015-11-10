#include "UIManager.h"


CUIManager::CUIManager(QWidget *parent) :
    QWidget(parent),
    m_pAlertUI(NULL),
    m_pAudioPassThru(NULL),
    m_pChoicesetVR(NULL),
    m_pChoiceset(NULL),
    m_pCommand(NULL),
    m_pScrollMsg(NULL),
    m_pShow(NULL),
    m_pSlider(NULL),
    m_pNotify(NULL),
    m_pCurUI(NULL),
    m_pCurPop(NULL)
{


    m_pAppLink = new AppLink;
    m_pAlertUI = new CAlertUI;
    m_pAudioPassThru = new CAudioPassThru;
    m_pChoicesetVR = new CChoicesetVR;
    m_pChoiceset = new Choiceset;
    m_pCommand = new Command;
    m_pScrollMsg = new CScrollMsg;
    m_pShow = new Show;
    m_pNotify = new Notify;
    m_pSlider = new Slider;
    m_pCurUI = m_pAppLink;

    connect(this,SIGNAL(onAppShowSignal(int)),this,SLOT(AppShowSlot(int)));
    connect(this,SIGNAL(onAppCloseSignal()),this,SLOT(AppCloseSlot()));
    connect(this,SIGNAL(onAppRefreshSignal()),this,SLOT(AppRefreshSlot()));

    connect(this,SIGNAL(onTestStartSignal()),this,SLOT(onTestStartSlots()));
    connect(this,SIGNAL(onTestStopSignal()),this,SLOT(onTestStopSlots()));

   // connect(this,SIGNAL(flush()),this,SLOT(execRefreshSlots()));

    connect(m_pAppLink,SIGNAL(inAppSignals(int)),this,SLOT(inAppSlots(int)));
    connect(m_pAppLink,SIGNAL(findNewApp()),this,SLOT(findNewAppSlots()));

    connect(m_pShow,SIGNAL(moreClicked()),this,SLOT(moreClickedSlots()));
    connect(m_pShow,SIGNAL(returnAppLink()),this,SLOT(returnAppLinkSlots()));
    connect(m_pShow,SIGNAL(softButtonClicked(int, int)),this,SLOT(softButtonClickedSlots(int, int)));

    connect(m_pCommand,SIGNAL(returnShow()),this,SLOT(returnShowSlots()));
    connect(m_pCommand,SIGNAL(exitApp()),this,SLOT(exitAppSlots()));
    connect(m_pCommand,SIGNAL(commandClick(int)),this,SLOT(commandClickSlots(int)));

    connect(m_pAlertUI,SIGNAL(alertAbort(int,int)),this,SLOT(alertAbortSlots(int,int)));
    connect(m_pAlertUI,SIGNAL(softButtonClicked(int,int)),this,SLOT(softButtonClickedSlots(int,int)));

    connect(m_pScrollMsg,SIGNAL(scrollMsgAbort(int,int)),this,SLOT(scrollMsgAbortSlots(int,int)));
    connect(m_pScrollMsg,SIGNAL(softButtonClicked(int,int)),this,SLOT(softButtonClickedSlots(int,int)));

    connect(m_pAudioPassThru,SIGNAL(audioPassThruHide(int,int)),this,SLOT(audioPassThruHideSlots(int,int)));


    connect(m_pChoiceset,SIGNAL(menuClicked(int,int,int)),this,SLOT(menuClickedSlots(int,int,int)));
    connect(m_pChoicesetVR,SIGNAL(VRmenuClicked(int,int,int)),this,SLOT(VRmenuClickedSlots(int,int,int)));

    connect(m_pSlider,SIGNAL(sliderClicked(int,int,int)),this,SLOT(sliderClickedSlots(int,int,int)));



    connect(m_pAppLink,SIGNAL(menuBtnClicked(QString)),this,SLOT(menuBtnClickedSlots(QString)));
    connect(m_pShow,SIGNAL(menuBtnClicked(QString)),this,SLOT(menuBtnClickedSlots(QString)));
    connect(m_pCommand,SIGNAL(menuBtnClicked(QString)),this,SLOT(menuBtnClickedSlots(QString)));


}

CUIManager::~CUIManager()
{

}

void CUIManager::init()
{
//    this->AppShowSlot(ID_APPLINK);
    m_pShow->testShow();

//    m_pAlertUI->setParent(m_pShow);
//    m_pAlertUI->testShow();

//    m_pAudioPassThru->setParent(m_pShow);
//    m_pAudioPassThru->testShow();

//    m_pChoiceset->testShow();
//    m_pChoicesetVR->setParent(m_pChoiceset);
//    m_pChoicesetVR->testShow();

//    m_pScrollMsg->setParent(m_pShow);
//    m_pScrollMsg->testShow();

//    m_pSlider->setParent(m_pShow);
//    m_pSlider->testShow();

//    m_pNotify->setParent(m_pShow);
//    m_pNotify->testShow();
}

//ID_APPLINK = 0,
//ID_SHOW,
//ID_ALERT,
//ID_AUDIOPASSTHRU,
//ID_CHOICESET,
//ID_CHOICESETVR,
//ID_SCROLLMSG,
//ID_COMMAND,
//ID_SLIDER,
//ID_NOTIFY
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
    std::string str_url = m_pAppDataInterface->getUrlString();
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
    if (m_pCurUI)
    {
        m_pCurUI->hide();
    }
    if (m_pCurPop)
    {
        m_pCurPop->setParent(NULL);
        m_pCurPop->hide();
        m_pCurPop = NULL;
    }

    switch (type)
    {
    case ID_APPLINK:    // 0
        m_pCurUI = m_pAppLink;
        break;
    case ID_SHOW:       // 1
        m_pCurUI = m_pShow;
        break;
    case ID_ALERT:      // 2
        m_pCurPop = m_pAlertUI;
        break;
    case ID_AUDIOPASSTHRU:  // 3
        m_pCurPop = m_pAudioPassThru;
        break;
    case ID_CHOICESET:  // 4
        m_pCurUI = m_pChoiceset;
        break;
    case ID_CHOICESETVR:    // 5
        m_pCurPop = m_pChoicesetVR;
        break;
    case ID_SCROLLMSG:  // 6
        m_pCurPop = m_pScrollMsg;
        break;
    case ID_COMMAND:    // 7
        m_pCurUI = m_pCommand;
        break;
    case ID_SLIDER:
        m_pCurPop = m_pSlider;
        break;
    case ID_NOTIFY:
        m_pCurPop = m_pNotify;
        break;
    }



    if (m_pCurUI)
    {
        if (m_pCurUI == m_pAppLink)
        {
            m_pAppLink->execShow(m_pAppListInterface);
        }
        else
        {
            m_pCurUI->execShow(m_pAppDataInterface);
        }
    }
    if (m_pCurPop)
    {
        m_pCurPop->setParent(m_pCurUI);
        m_pCurPop->execShow(m_pAppDataInterface);
    }
}

void CUIManager::AppCloseSlot()
{

}

void CUIManager::AppRefreshSlot()
{

}

void CUIManager::inAppSlots(int appID)
{
    qDebug()<<appID<<"+++++";
    m_i_appID = appID;
    m_pAppListInterface->OnAppActivated(appID);
    this->AppShowSlot(ID_SHOW);

}



void CUIManager::findNewAppSlots()
{
    this->AppShowSlot(ID_NOTIFY);
}

void CUIManager::moreClickedSlots()
{

    this->AppShowSlot(ID_COMMAND);
}

void CUIManager::returnAppLinkSlots()
{
    //_D("appID = %d\n",m_i_appID);
    m_pAppListInterface->OnApplicationOut(m_i_appID);
    this->AppShowSlot(ID_APPLINK);
}

void CUIManager::returnShowSlots()
{
    this->AppShowSlot(ID_SHOW);
}

void CUIManager::exitAppSlots()
{
    m_pAppListInterface->OnApplicationExit(m_i_appID);
    this->AppShowSlot(ID_APPLINK);
}

void CUIManager::softButtonClickedSlots(int btID, int mode)
{
    //_D("btID = %d:%d\n",btID,mode);
    m_pAppDataInterface->OnSoftButtonClick(btID,mode);
}

void CUIManager::commandClickSlots(int cmdID)
{
    //_D("appID = %d:%d\n",m_i_appID,cmdID);
    m_pAppDataInterface->OnCommandClick(m_i_appID, cmdID);
}

void CUIManager::alertAbortSlots(int alertID, int reason)
{
    //_D("alertID=%d, reason=%d\n",alertID,reason);
    m_pAppDataInterface->OnAlertResponse(alertID,reason);
}

void CUIManager::scrollMsgAbortSlots(int smID, int reason)
{
    //_D("smID=%d, reason=%d\n",smID,reason);
    m_pAppDataInterface->OnScrollMessageResponse(smID, reason);
}

void CUIManager::audioPassThruHideSlots(int audioPassThruId, int code)
{
    //_D("appID=%d:%d:%d\n",m_i_appID,audioPassThruId,code);
    m_pAppDataInterface->OnPerformAudioPassThru(m_i_appID, audioPassThruId, code);
    m_pAppDataInterface->OnVRCancelRecord();
}

void CUIManager::menuClickedSlots(int code, int performInteractionID, int choiceID)
{
    //_D("code=%d:%d:%d\n",code,performInteractionID,choiceID);
    m_pAppDataInterface->OnPerformInteraction(code, performInteractionID, choiceID);
    this->AppShowSlot(ID_SHOW);
}
void CUIManager::VRmenuClickedSlots(int code, int performInteractionID, int choiceID)
{
    //_D("code=%d:%d:%d\n",code,performInteractionID,choiceID);
    m_pAppDataInterface->OnPerformInteraction(code, performInteractionID, choiceID);
    this->AppShowSlot(ID_SHOW);
}

void CUIManager::sliderClickedSlots( int code, int sliderid, int sliderPosition)
{
    //_D("code=%d:%d:%d\n",code,sliderid,sliderPosition);
    m_pAppDataInterface->OnSliderResponse(code, sliderid,sliderPosition);
    this->AppShowSlot(ID_SHOW);
}
void CUIManager::setAppListInterface(AppListInterface* pInterface)
{
    m_pAppListInterface = pInterface;
    m_pAppDataInterface = m_pAppListInterface->getAppDataInterface();
}

void CUIManager::menuBtnClickedSlots(QString btnText)
{

    //_D("%s\n",btnText.toUtf8().data());
    if(m_pCurUI == m_pAppLink)
        return;

    m_pAppDataInterface->OnMenuBtnClick(btnText.toUtf8().data());

    if("ListButton" == btnText)
    {
        this->AppShowSlot(ID_AUDIOPASSTHRU);
        //ts.speak("请说一个指令");
        //while(ts.isSpeaking())
           // waitMSec(100);
        m_pAppDataInterface->OnVRStartRecord();
    }
}

void CUIManager::setMediaColckTimer(Json::Value jsonObj)
{
    m_pShow->setMediaColckTimer(jsonObj);
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
    ts.speak(strText.data());
    while(ts.isSpeaking())
        waitMSec(100);

    switch(VRID)
    {
    case ID_CANCEL:
        m_pAppDataInterface->OnPerformAudioPassThru(m_i_appID, 0, PERFORMAUDIOPASSTHRU_CANCEL);
        m_pAppDataInterface->OnVRCancelRecord();
        m_pAudioPassThru->hide();
        break;
    case ID_HELP:
        m_pAppDataInterface->OnPerformAudioPassThru(m_i_appID, 0, PERFORMAUDIOPASSTHRU_DONE);
        m_pAppDataInterface->OnVRCancelRecord();
        m_pAudioPassThru->hide();
        break;
    case ID_EXIT:
        m_pAppDataInterface->OnPerformAudioPassThru(m_i_appID, 0, PERFORMAUDIOPASSTHRU_DONE);
        m_pAppDataInterface->OnVRCancelRecord();
        m_pAudioPassThru->hide();
        m_pAppListInterface->OnApplicationOut(m_i_appID);
        this->onAppShow(ID_APPLINK);
        break;
    case ID_SWITCHAPP:
        m_pAppDataInterface->OnPerformAudioPassThru(m_i_appID, 0, PERFORMAUDIOPASSTHRU_DONE);
        m_pAppDataInterface->OnVRCancelRecord();
        m_pAudioPassThru->hide();
        break;
    default:
        m_pAudioPassThru->hide();
        break;
    }
}

void CUIManager::switchNewApp(int newAppID)
{
    m_pAppListInterface->OnApplicationOut(m_i_appID);

    m_i_appID = newAppID;
    m_pAppListInterface->OnAppActivated(m_i_appID);
    this->AppShowSlot(ID_SHOW);
}
