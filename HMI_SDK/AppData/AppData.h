#ifndef APPDATA_H
#define APPDATA_H

#include "Connect/connect.h"
#include "Connect/SDLConnector.h"
#include "AppData/AppDataInterface.h"
#include "UI/UIManager.h"

class AppData : public AppDataInterface
{
public:
    AppData();

//    void start();
    void setUIManager(UIInterface *pcallBack);

    Json::Value getShowData();
    std::vector<SMenuCommand> getCommandList();
    std::vector<SMenuCommand> getCommandList(int subMenuID);
    Json::Value getAlertJson();
    Json::Value getSlider();
    Json::Value getScrollableMsgJson();
    Json::Value getAudioPassThruJson();
    Json::Value getInteractionJson();
    std::string getUrlString();

    void OnSoftButtonClick(int sbID, int mode);
    void OnCommandClick(int appID,int cmdID);
    void OnAlertResponse(int alertID, int reason);
    void OnScrollMessageResponse(int smID, int reason);
    void OnSliderResponse( int code, int sliderid, int sliderPosition);
    void OnTTSSpeek(int speekID, int code);
    void OnPerformAudioPassThru(int appID, int performaudiopassthruID, int code);
    void OnPerformInteraction(int code, int performInteractionID, int choiceID);

    void OnMenuBtnClick(std::string btnText);
    void OnVRStartRecord();
    void OnVRCancelRecord();

    void recvFromServer(Json::Value);

    void setCurrentAppID(int appID);
    void setCurrentAppName(std::string appName);

private:


    UIInterface *m_pUIManager;

    Json::Value m_json_show;
//    SShow m_struct_show;
    std::vector <SMenuCommand > m_vec_scommand;

    Json::Value m_json_alert;
    Json::Value m_json_slider;
    Json::Value m_json_scrollableMessage;
    Json::Value m_json_audioPassThru;
    Json::Value m_json_interaction;

    void uiShow(Json::Value jsonObj);
    void addCommand(Json::Value jsonObj);
    void addSubMenu(Json::Value jsonObj);
    void delCommand(Json::Value jsonObj);
    void delSubMenu(Json::Value jsonObj);
    void alert(Json::Value jsonObj);
    void slider(Json::Value jsonObj);
    void scrollableMessage(Json::Value jsonObj);
    void performAudioPassThru(Json::Value jsonObj);
    void performInteraction(Json::Value jsonObj);
    void systemRequest(Json::Value jsonObj);

    Json::Value m_videoStreamJson;
    void videoStreamStart(Json::Value jsonObj);
    void videoStreamStop(Json::Value jsonObj);

    int m_i_currentAppID;
    std::string m_str_currentAppName;
    int m_i_currentUIID;
};

#endif // APPDATA_H
