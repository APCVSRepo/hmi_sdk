#ifndef APPDATA_H
#define APPDATA_H

#include "Connect/connect.h"
#include "Connect/SDLConnector.h"
#include "AppData/AppDataInterface.h"
#include "UIInterface.h"

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
    Json::Value getMediaClockJson();

    int getCurUI();

    void OnShowCommand();
    void OnSoftButtonClick(int sbID, int mode);
    void OnCommandClick(int cmdID);
    void OnAlertResponse(int reason);
    void OnScrollMessageResponse(int reason);
    void OnSliderResponse( int code, int sliderPosition);
    void OnTTSSpeek(int code);
    void OnPerformAudioPassThru(int code);
    void OnPerformInteraction(int code, int choiceID);

    void OnMenuBtnClick(std::string btnText);
    void OnVRStartRecord();
    void OnVRCancelRecord();

    void recvFromServer(Json::Value);

private:
    UIInterface *m_pUIManager;
    std::vector<int> m_vecUIStack;
    void ShowUI(int);
    void ShowPreviousUI();

    Json::Value m_json_show;
//    SShow m_struct_show;
    std::vector <SMenuCommand > m_vec_scommand;

    Json::Value m_json_alert;
    Json::Value m_json_slider;
    Json::Value m_json_scrollableMessage;
    Json::Value m_json_audioPassThru;
    Json::Value m_json_interaction;
    Json::Value m_json_mediaclock;

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

public:
    int m_iAppID;
    std::string m_szAppName;
};

#endif // APPDATA_H
