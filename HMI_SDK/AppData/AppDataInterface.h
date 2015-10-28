#ifndef APPDATAINTERFACE_H
#define APPDATAINTERFACE_H

#include "json/json.h"
#include <string.h>
#include <vector>

typedef struct command{
    int i_appID;
    int i_cmdID;
    int i_menuID;
    std::string str_menuName;
    int i_parentID;
    int i_position;
    command()
    {
        i_appID = 0;
        i_cmdID = 0;
        i_menuID = 0;
        str_menuName.clear();
        i_parentID = 0;
        i_position = 0;
    }
}SMenuCommand;

class AppDataInterface
{
public:
    virtual Json::Value getShowData() = 0;
    virtual std::vector<SMenuCommand> getCommandList() = 0;
    virtual std::vector<SMenuCommand> getCommandList(int subMenuID) = 0;
    virtual Json::Value getAlertJson() = 0;
    virtual Json::Value getSlider() = 0;
    virtual Json::Value getScrollableMsgJson() = 0;
    virtual Json::Value getAudioPassThruJson() = 0;
    virtual Json::Value getInteractionJson() = 0;
    virtual std::string getUrlString() = 0;

    virtual void OnSoftButtonClick(int sbID, int mode) = 0;
    virtual void OnCommandClick(int appID,int cmdID) = 0;
    virtual void OnAlertResponse(int alertID, int reason) = 0;
    virtual void OnScrollMessageResponse(int smID, int reason) = 0;
    virtual void OnSliderResponse( int code, int sliderid, int sliderPosition) = 0;
    virtual void OnTTSSpeek(int speekID, int code) = 0;
    virtual void OnPerformAudioPassThru(int appID, int performaudiopassthruID, int code) = 0;
    virtual void OnPerformInteraction(int code, int performInteractionID, int choiceID) = 0;

    virtual void OnMenuBtnClick(std::string btnText) = 0;
    virtual void OnVRStartRecord() = 0;
    virtual void OnVRCancelRecord() = 0;
};

#endif // APPDATAINTERFACE_H
