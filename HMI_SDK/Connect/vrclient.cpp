#include <Include/global_first.h>
#include <Connect/vrclient.h>


#include <iostream>
#include <string>
#include "json/json.h"

vrClient::vrClient() : Channel(400,"VR")
{

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


void vrClient::onRequest(Json::Value &request)
{
    std::string method = request["method"].asString();
    int id = request["id"].asInt();
    if (method == "VR.GetSupportedLanguages")
    {
        sendResult(id,"GetSupportedLanguages");
    }
    else if (method == "VR.GetLanguage")
    {
        sendResult(id,"GetLanguage");
    }
    else if (method == "VR.ChangeRegistration")
    {
        sendResult(id,"ChangeRegistration");
    }
    else if(method == "VR.IsReady")
    {
        sendResult(id,"IsReady");
    }
    else if(method == "VR.GetCapabilities")
    {
        sendResult(id,"GetCapabilities");
    }
    else if (method == "VR.AddCommand")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"AddCommand",result);
    }
    else if(method == "VR.DeleteCommand")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"DeleteCommand",result);
    }
    else if(method=="VR.PerformInteraction")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"PerformInteraction",result);
    }
    else
    {
        Channel::onRequest(request);
    }
}

