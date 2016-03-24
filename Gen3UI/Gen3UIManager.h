#ifndef CGEN3UIMANAGER_H
#define CGEN3UIMANAGER_H
//#include "UI/Alert/AlertUI.h"
//#include "UI/AudioPassThru/AudioPassThru.h"
//#include "UI/Choiceset/Choiceset.h"
//#include "UI/Choiceset/ChoicesetVR.h"
//#include "UI/Command/Command.h"
//#include "UI/ScrollableMessage/ScrollMsg.h"
//#include "UI/Show/Show.h"
//#include "UI/Config/Config.h"
//#include "UI/Slider/Slider.h"
//#include "UI/Notify/Notify.h"
//#include "UI/Common/AppBase.h"
//#include "UI/TextSpeech/textspeech.h"
#include "UIInterface.h"
#include "AppListInterface.h"
#include "QMessageBox"
//#include "Gen3UI/MainWindow/MainWindow.h"


#ifdef SDL_SUPPORT_LIB
//#include "AudioTrans/MspVRAudio.h"
#endif

class CGen3UIManager : public QWidget, public UIInterface
{
    Q_OBJECT
public:
    explicit CGen3UIManager(AppListInterface * pList, QWidget *parent = NULL);
    ~CGen3UIManager();

    void onAppActive();
    void onAppStop();
    //hmi
    void onAppShow(int type);

    void onVideoStreamStart();
    void onVideoStreamStop();

    void tsSpeak(int VRID, std::string strText);

    void OnEndAudioPassThru();
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
    //msp_vr_audio *m_MspVR;
#endif
    //TextSpeech ts;
    void waitMSec(int ms);
};

#endif // CUIMANAGER_H
