#include "HMISDK/include/global_first.h"
#include "HMISDK/include/connect/uiclient.h"

#include <iostream>
#include <string>
#include "HMISDK/include/json/json.h"

uiClient::uiClient() : Channel("UI")
{
    m_iIDStart = 400;
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

void uiClient::onRequest(Json::Value request)
{
    std::string method = request["method"].asString();

    if(method == "UI.SetGlobalProperties")
    {
        sendUIResult(request["id"].asInt(), request["method"].asString());
    }
    else if(method == "UI.ResetGlobalProperties")
    {
        sendUIResult(request["id"].asInt(), request["method"].asString());
    }
    else if(method == "UI.ChangeRegistration")
    {
        sendUIResult(request["id"].asInt(), request["method"].asString());
    }
    else if(method == "UI.SetAppIcon")
    {
        sendUIResult(request["id"].asInt(), request["method"].asString());
    }
    else if(method == "UI.GetSupportedLanguages")
    {
        getSupportedLanguages(request["id"].asInt());
    }
    else if(method == "UI.GetLanguage")
    {
        getLanguage(request["id"].asInt());
    }
    else if(method == "UI.GetCapabilities")
    {
        getCapabilities(request["id"].asInt());
    }
    else if(method == "UI.IsReady")
    {
        isReady(request["id"].asInt());
    }
    else if(method == "UI.ClosePopUp")
    {
        closePopUp(request["id"].asInt());
    }
    else if(method == "UI.ShowVrHelp")
    {
        sendUIResult(request["id"].asInt(), request["method"].asString());
    }
    else
    {
        if (method == "UI.Alert")
        {
            onSystemContext("ALERT");
        }
        else if(method == "UI.Show")
        {
            sendUIResult(request["id"].asInt(), request["method"].asString());
        }
        else if(method == "UI.AddCommand")
        {
            sendUIResult(request["id"].asInt(), request["method"].asString());
        }
        else if(method == "UI.DeleteCommand")
        {
            sendUIResult(request["id"].asInt(), request["method"].asString());
        }
        else if(method == "UI.AddSubMenu")
        {
            sendUIResult(request["id"].asInt(), request["method"].asString());
        }
        else if(method == "UI.DeleteSubMenu")
        {
            sendUIResult(request["id"].asInt(), request["method"].asString());
        }
        else if(method == "UI.PerformInteraction")
        {
            //sendUIResult(request["id"].asInt(), request["method"].asString());
        }
        else if(method == "UI.SetMediaClockTimer")
        {
            sendMediaClockTimerResponse(request["id"].asInt());
        }
        else if(method == "UI.PerformAudioPassThru")
        {
            startRecording(request);
        }
        else if(method == "UI.EndAudioPassThru")
        {
            sendUIResult(request["id"].asInt(), request["method"].asString());
        }
        m_pCallback->onRequest(request);
    }
}


void uiClient::sendUIResult(int id, std::string method)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = method;

    root["result"] = result;
    SendJson(root);
}

void uiClient::sendMediaClockTimerResponse(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "UI.SetMediaClockTimer";

    root["result"] = result;
    SendJson(root);
}

void uiClient::getSupportedLanguages(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "UI.GetSupportedLanguages";
    result["languages"] = m_StaticConfigJson["sdlLanguagesList"];

    root["result"] = result;
    SendJson(root);
}

void uiClient::getLanguage(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "UI.GetLanguage";
    result["languages"] = m_StaticConfigJson["hmiUILanguage"];

    root["result"] = result;
    SendJson(root);
}

void uiClient::getCapabilities(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "UI.GetCapabilities";

    root["result"] = m_StaticConfigJson["uiGetCapabilitiesResult"];;
    SendJson(root);
}

void uiClient::isReady(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "UI.IsReady";
    result["available"] = true;

    root["result"] = result;
    SendJson(root);
}

void uiClient::closePopUp(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "UI.ClosePopUp";

    root["result"] = result;
    SendJson(root);
}

void uiClient::onSystemContext(std::string systemContext)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "UI.OnSystemContext";

    params["systemContext"] = systemContext;

    root["params"] = params;
    SendJson(root);
}

void uiClient::startRecording(Json::Value request)
{
    Json::Value request_params;
    Json::Value root;
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
    root["jsonrpc"] = "2.0";
    root["method"] = "PerformAudioPassThruStart";
    root["params"] = params;
    SendJson(root);
}
