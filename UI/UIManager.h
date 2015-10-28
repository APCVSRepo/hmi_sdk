#ifndef CUIMANAGER_H
#define CUIMANAGER_H
#include "UI/AppLink.h"
#include "UI/Alert/AlertUI.h"
#include "UI/AudioPassThru/AudioPassThru.h"
#include "UI/Choiceset/Choiceset.h"
#include "UI/Choiceset/ChoicesetVR.h"
#include "UI/Command/Command.h"
#include "UI/ScrollableMessage/ScrollMsg.h"
#include "UI/Show/Show.h"
#include "UI/Config/Config.h"
#include "UI/Slider/Slider.h"
#include "UI/Notify/Notify.h"
#include "UI/Common/AppBase.h"
#include "UI/Common/PopBase.h"
#include "UI/TextSpeech/textspeech.h"
#include "UIInterface.h"
#include "AppData/AppListInterface.h"
#include "QMessageBox"
#define VIDEO_TEST
#ifdef VIDEO_TEST
#include "UI/VideoStream/VideoStream.h"
#endif

extern Config g_config;

class CUIManager : public QWidget, public UIInterface
{
    Q_OBJECT
public:
    explicit CUIManager(QWidget *parent = 0);
    ~CUIManager();

    void init();
    void onAppShow(int type);
    void onAppActivated(AppDataInterface* pInterface);
    void onAppClose();
    void onAppRefresh();

    void onTestVideoStreamStart();
    void onTestVideoStreamStop();

    void setMediaColckTimer(Json::Value jsonObj);

    void setAppListInterface(AppListInterface* pInterface);

    void tsSpeak(int VRID, std::string strText);
    void switchNewApp(int newAppID);

signals:
    void onAppShowSignal(int type);
    void onAppActivatedSignal(AppDataInterface* pInterface);
    void onAppCloseSignal();
    void onAppRefreshSignal();

    void onTestStartSignal();
    void onTestStopSignal();
public slots:

    void AppShowSlot(int type);
    void AppCloseSlot();
    void AppRefreshSlot();

    void inAppSlots(int appID);
    void findNewAppSlots();
    void moreClickedSlots();

    void returnAppLinkSlots();
    void returnShowSlots();
    void exitAppSlots();

    void softButtonClickedSlots(int btID, int mode);
    void commandClickSlots(int cmdID);

    void alertAbortSlots(int alertID, int reason);
    void scrollMsgAbortSlots(int scrollMsgID, int reason);
    void audioPassThruHideSlots(int audioPassThruId, int code);
    void menuClickedSlots(int code, int performInteractionID, int choiceID);
    void VRmenuClickedSlots(int code, int performInteractionID, int choiceID);
    void sliderClickedSlots( int code, int sliderid, int sliderPosition);

    void onTestStartSlots();
    void onTestStopSlots();

    void menuBtnClickedSlots(QString btnText);

private:
    AppLink *m_pAppLink;
    CAlertUI *m_pAlertUI;
    CAudioPassThru *m_pAudioPassThru;
    CChoicesetVR   *m_pChoicesetVR;
    Choiceset *m_pChoiceset;
    Command *m_pCommand;
    CScrollMsg *m_pScrollMsg;
    Show *m_pShow;
    Slider *m_pSlider;
    Notify *m_pNotify;
    AppBase *m_pCurUI;
    CPopBase *m_pCurPop;

    AppDataInterface* m_pAppDataInterface;
    AppListInterface* m_pAppListInterface;

#ifdef VIDEO_TEST
    VideoStream m_videoStreamWidget;
#endif
    int m_i_appID;

    TextSpeech ts;
    void waitMSec(int ms);


};

#endif // CUIMANAGER_H
