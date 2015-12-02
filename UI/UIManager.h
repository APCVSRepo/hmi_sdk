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

#include "Common/MainMenue.h"


class CUIManager : public QWidget, public UIInterface
{
    Q_OBJECT
public:
    explicit CUIManager(QWidget *parent = NULL);
    ~CUIManager();

    //start sdl
    void  initSDL();
#ifdef SDL_SUPPORT_LIB
    static void *SDLStartThread(void *arg);
    static void SDLKillThread();
    static bool FileCopyToConfigdir(const char *dir);
#endif
    //hmi
    void initAppHMI();
    void showMainUI();
    void onAppShow(int type);
    void onAppActivated(AppDataInterface* pInterface);
    void onAppClose();
    void onAppRefresh();

    void onTestVideoStreamStart();
    void onTestVideoStreamStop();

    void setMediaColckTimer(Json::Value jsonObj);

    void tsSpeak(int VRID, std::string strText);
    void switchNewApp(int newAppID);

signals:
    void finishMainHMI();
    void onAppShowSignal(int type);
    void onAppActivatedSignal(AppDataInterface* pInterface);
    void onAppCloseSignal();
    void onAppRefreshSignal();

    void onTestStartSignal();
    void onTestStopSignal();
public slots:
    void onMainHMIStart();
    void AppShowSlot(int type);
    void AppCloseSlot();
    void AppRefreshSlot();


    void onTestStartSlots();
    void onTestStopSlots();


private:
    MainMenue *m_MainMenu;
    DataManager *manager;

#ifdef VIDEO_TEST
    VideoStream m_videoStreamWidget;
#endif
    TextSpeech ts;
    void waitMSec(int ms);
};

#endif // CUIMANAGER_H
