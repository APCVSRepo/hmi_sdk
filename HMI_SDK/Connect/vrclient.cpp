#include <global_first.h>
#include <connect/vrclient.h>


#include <iostream>
#include <string>
#include "json/json.h"

vrClient::vrClient() : Channel("VR")
{
    m_iIDStart = 500;
}

vrClient::~vrClient()
{

}

void vrClient::onRegistered()
{
    SubscribeToNotification("VR.VRCancel");
	SubscribeToNotification("VR.VRStatus");
	SubscribeToNotification("VR.VRCommandHelp");
	SubscribeToNotification("VR.VRCommandTTS");
	SubscribeToNotification("VR.VRExitApp");
	SubscribeToNotification("VR.VRSwitchApp");
	SubscribeToNotification("VR.VRResult");
}

void vrClient::onUnregistered()
{
    UnsubscribeFromNotification("VR.VRCancel");
	UnsubscribeFromNotification("VR.VRStatus");
	UnsubscribeFromNotification("VR.VRCommandHelp");
	UnsubscribeFromNotification("VR.VRCommandTTS");
	UnsubscribeFromNotification("VR.VRExitApp");
	UnsubscribeFromNotification("VR.VRSwitchApp");
	UnsubscribeFromNotification("VR.VRResult");
}


void vrClient::onRequest(Json::Value request)
{
    std::string method = request["method"].asString();

    if (method == "VR.GetSupportedLanguages")
    {
        getSupportedLanguagesAction(request["id"].asInt());
    }
    else if (method == "VR.GetLanguage")
    {
        getLanguage(request["id"].asInt());
    }
    else if (method == "VR.ChangeRegistration")
    {
        sendVRResult(0, request["id"].asInt(), request["method"].asString());
    }
    else if(method == "VR.IsReady")
    {
        isReady(request["id"].asInt());
    }
    else if(method == "VR.GetCapabilities")
    {
        //sendVRResult(0, request["id"].asInt(), request["method"].asString());
    }
    else
    {
        if (method == "VR.AddCommand")
        {
            sendVRResult(0, request["id"].asInt(), request["method"].asString());
        }
        else if(method == "VR.DeleteCommand")
        {
            sendVRResult(0, request["id"].asInt(), request["method"].asString());
        }
		m_pCallback->onRequest(request);
    }
}

void vrClient::sendVRResult(int resultCode, int id, std::string method)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = resultCode;
    result["method"] = method;

    root["result"] = result;
	SendJson(root);
}

void vrClient::getSupportedLanguagesAction(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "VR.GetSupportedLanguages";
    result["languages"] = m_StaticConfigJson["sdlLanguagesList"];

    root["result"] = result;
	SendJson(root);
}

void vrClient::getLanguage(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "VR.GetLanguage";
    result["languages"] = m_StaticConfigJson["hmiVRLanguage"];

    root["result"] = result;
	SendJson(root);
}

void vrClient::isReady(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "VR.IsReady";
    result["available"] = true;

    root["result"] = result;
	SendJson(root);
}
