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
    ID_MEDIACLOCK,
    ID_MAIN,
    ID_UI_MAX
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

    virtual void onVideoStreamStart() = 0;
    virtual void onVideoStreamStop() = 0;

    virtual void tsSpeak(int VRID, std::string strText) = 0;
};

#endif // UIINTERFACE_H
