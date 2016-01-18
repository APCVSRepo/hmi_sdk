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
#include "UI/TextSpeech/textspeech.h"
#include "UIInterface.h"
#include "AppData/AppListInterface.h"
#include "QMessageBox"


#include "Common/MainMenu.h"
#ifdef SDL_SUPPORT_LIB
#include "AudioTrans/MspVRAudio.h"
#endif

class CUIManager : public QWidget, public UIInterface
{
    Q_OBJECT
public:
    explicit CUIManager(AppListInterface * pList, QWidget *parent = NULL);
    ~CUIManager();

    void onAppActive();
    void onAppStop();
    //hmi
    void onAppShow(int type);

    void onVideoStreamStart();
    void onVideoStreamStop();

    void tsSpeak(int VRID, std::string strText);

signals:
    //void finishMainHMI();
    void onAppShowSignal(int type);
    void onAppActivatedSignal(AppDataInterface* pInterface);

    void onVideoStartSignal();
    void onVideoStopSignal();

public slots:
    void initAppHMI();
    void AppShowSlot(int type);


    void onVideoStartSlots();
    void onVideoStopSlots();


private:
    QWidget * m_vUIWidgets[ID_UI_MAX];
    int m_iCurUI;
    AppListInterface * m_pList;

#ifdef SDL_SUPPORT_LIB
    msp_vr_audio *m_MspVR;
#endif
    TextSpeech ts;
    void waitMSec(int ms);
};

#endif // CUIMANAGER_H
