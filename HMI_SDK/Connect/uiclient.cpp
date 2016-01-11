#include <Include/global_first.h>
#include <Connect/uiclient.h>

#include <iostream>
#include <string>
#include "json/json.h"
#include "Config/Config.h"

uiClient::uiClient() : Channel(600,"UI")
{

}

uiClient::~uiClient()
{

}

void uiClient::onRegistered()
{
    SubscribeToNotification("UI.ShowNotification");
    SubscribeToNotification("UI.CreateInteractionChoiceSet");
    SubscribeToNotification("UI.DeleteInteractionChoiceSet");
    SubscribeToNotification("UI.SubscribeButton");
    SubscribeToNotification("UI.UnsubscribeButton");
}

void uiClient::onUnregistered()
{
    UnsubscribeFromNotification("UI.ShowNotification");
    UnsubscribeFromNotification("UI.CreateInteractionChoiceSet");
    UnsubscribeFromNotification("UI.DeleteInteractionChoiceSet");
    UnsubscribeFromNotification("UI.SubscribeButton");
    UnsubscribeFromNotification("UI.UnsubscribeButton");
}



void uiClient::onRequest(Json::Value &request)
{
    std::string method = request["method"].asString();
    int id = request["id"].asInt();
    if(method == "UI.SetGlobalProperties")
    {
        sendResult(id,"SetGlobalProperties");
    }
    else if(method == "UI.ResetGlobalProperties")
    {
        sendResult(id,"ResetGlobalProperties");
    }
    else if(method == "UI.ChangeRegistration")
    {
        sendResult(id,"ChangeRegistration");
    }
    else if(method == "UI.SetAppIcon")
    {
        sendResult(id,"SetAppIcon");
    }
    else if(method == "UI.GetSupportedLanguages")
    {
        sendResult(id,"GetSupportedLanguages");
    }
    else if(method == "UI.GetLanguage")
    {
        sendResult(id,"GetLanguage");
    }
    else if(method == "UI.GetCapabilities")
    {
        sendResult(id,"GetCapabilities");
    }
    else if(method == "UI.IsReady")
    {
        sendResult(id,"IsReady");
    }
    else if(method == "UI.ClosePopUp")
    {
        sendResult(id,"ClosePopUp");
    }
    else if(method == "UI.ShowVrHelp")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"ShowVrHelp",result);
    }
    else if (method == "UI.Alert")
    {
        m_pCallback->onRequest(request);

        Json::Value params;
        params["systemContext"] = "ALERT";
        sendNotification("UI.OnSystemContext",params);
    }
    else if(method == "UI.Show")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"Show",result);
    }
    else if(method=="UI.ScrollableMessage")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"ScrollableMessage",result);
    }
    else if(method == "UI.AddCommand")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"AddCommand",result);
    }
    else if(method == "UI.DeleteCommand")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"DeleteCommand",result);
    }
    else if(method == "UI.AddSubMenu")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"AddSubMenu",result);
    }
    else if(method == "UI.DeleteSubMenu")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"DeleteSubMenu",result);
    }
    else if(method == "UI.PerformInteraction")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"PerformInteraction",result);
    }
    else if(method == "UI.SetMediaClockTimer")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"SetMediaClockTimer",result);
    }
    else if(method == "UI.PerformAudioPassThru")
    {
        m_pCallback->onRequest(request);
        sendNotification("UI.PerformAudioPassThru",startRecordingNotify(request));
    }
    else if(method == "UI.EndAudioPassThru")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"EndAudioPassThru",result);
    }
    else
    {
        Channel::onRequest(request);
    }
}


void uiClient::onSystemContext(std::string systemContext)
{
    Json::Value params;
    params["systemContext"] = systemContext;
    sendNotification("UI.OnSystemContext",params);
}


Json::Value uiClient::startRecordingNotify(Json::Value &request)
{
    Json::Value request_params;
    Json::Value params;
    request_params = request["params"];
    if(request_params.isMember("samplingRate")){
        params["samplingRate"] = request_params["samplingRate"];
    }
    if(request_params.isMember("bitsPerSample")){
        params["bitsPerSample"] = request_params["bitsPerSample"];
    }
    if(request_params.isMember("appID")){
        params["appID"] = request_params["appID"];
    }
    if(request_params.isMember("audioType")){
        params["audioType"] = request_params["audioType"];
    }
    if(request_params.isMember("maxDuration")){
        params["maxDuration"] = request_params["maxDuration"];
    }
    params["audioPassThruType"] = "RECORD_SEND";
    params["saveAudioPassThruFile"] = "-";
    params["SendAUduiPassThruFile"] = "-";
    return params;
}
