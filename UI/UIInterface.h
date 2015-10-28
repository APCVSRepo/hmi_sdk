#ifndef UIINTERFACE_H
#define UIINTERFACE_H
#include "AppData/AppDataInterface.h"

enum ShowType
{
    ID_APPLINK = 0,
    ID_SHOW,
    ID_ALERT,
    ID_AUDIOPASSTHRU,
    ID_CHOICESET,
    ID_CHOICESETVR,
    ID_SCROLLMSG,
    ID_COMMAND,
    ID_SLIDER,
    ID_NOTIFY
};

enum VRID
{
    ID_DEFAULT = 0,
    ID_CANCEL,
    ID_HELP,
    ID_EXIT,
    ID_SWITCHAPP
};

class UIInterface
{
public:
    virtual void onAppShow(int type) = 0;
    virtual void onAppActivated(AppDataInterface* pInterface) = 0;
    virtual void onAppClose() = 0;
    virtual void onAppRefresh() = 0;

    virtual void onTestVideoStreamStart() = 0;
    virtual void onTestVideoStreamStop() = 0;

    virtual void setMediaColckTimer(Json::Value jsonObj) = 0;
    virtual void tsSpeak(int VRID, std::string strText) = 0;

    virtual void switchNewApp(int newAppID) = 0;
};

#endif // UIINTERFACE_H
