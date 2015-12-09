#ifndef UIINTERFACE_H
#define UIINTERFACE_H
#include "AppData/AppDataInterface.h"

enum ShowType
{
    ID_APPLINK = 0,
    ID_CHOICESET,
    ID_COMMAND,
    ID_SHOW,
    ID_ALERT,
    ID_AUDIOPASSTHRU,
    ID_CHOICESETVR,
    ID_SCROLLMSG,
    ID_SLIDER,
    ID_NOTIFY,
    ID_APP_MAX
};

#define MIN_APP_BASE  ID_APPLINK
#define MAX_APP_BASE  (ID_SHOW+1)
#define MIN_POP_BASE  ID_ALERT
#define MAX_POP_BASE  (ID_APP_MAX)
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
    virtual void onAppClose() = 0;
    virtual void onAppRefresh() = 0;

    virtual void onTestVideoStreamStart() = 0;
    virtual void onTestVideoStreamStop() = 0;

    virtual void setMediaColckTimer(Json::Value jsonObj) = 0;
    virtual void tsSpeak(int VRID, std::string strText) = 0;
};

#endif // UIINTERFACE_H
