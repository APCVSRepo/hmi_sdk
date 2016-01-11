#include "Include/global_first.h"
#include "Connect/basecommunicationclient.h"

#include <iostream>
#include <string>
#include "json/json.h"

baseCommunicationClient::baseCommunicationClient() : Channel(500,"BasicCommunication")
{
}

baseCommunicationClient::~baseCommunicationClient()
{

}

void baseCommunicationClient::onRegistered()
{
	SubscribeToNotification("BasicCommunication.OnAppRegistered");
	SubscribeToNotification("BasicCommunication.OnAppUnregistered");
	SubscribeToNotification("BasicCommunication.PlayTone");
	SubscribeToNotification("BasicCommunication.SDLLog");
    sendNotification("BasicCommunication.OnReady");
}

void baseCommunicationClient::onUnregistered()
{
	UnsubscribeFromNotification("BasicCommunication.OnAppRegistered");
	UnsubscribeFromNotification("BasicCommunication.OnAppUnregistered");
	UnsubscribeFromNotification("BasicCommunication.PlayTone");
	UnsubscribeFromNotification("BasicCommunication.SDLLog");
}



void baseCommunicationClient::onRequest(Json::Value &request)
{
    std::string method = request["method"].asString();
    int  id= request["id"].asInt();
    if (method == "BasicCommunication.MixingAudioSupported")
    {
        sendResult(id,"MixingAudioSupported");
    }
    else if (method == "BasicCommunication.AllowAllApps")
    {
        sendResult(id,"AllowAllApps");
    }
    else if (method == "BasicCommunication.AllowApp")
    {
        sendResult(id,"AllowApp");
    }
    else if (method == "BasicCommunication.AllowDeviceToConnect")
    {
        sendResult(id,"AllowDeviceToConnect");
    }
    else if (method == "BasicCommunication.UpdateAppList")
    {
        sendResult(id,"UpdateAppList");
    }
    else if (method == "BasicCommunication.UpdateDeviceList")
    {
        sendResult(id,"UpdateDeviceList");
    }
    else if (method == "BasicCommunication.ActivateApp")
    {
        sendResult(id,"ActivateApp");
    }
    else if (method == "BasicCommunication.IsReady")
    {
        sendResult(id,"IsReady");
    }
    else if (method == "BasicCommunication.GetSystemInfo")
    {
        sendResult(id,"GetSystemInfo");
    }
    else
    {
        Channel::onRequest(request);
    }
}

void baseCommunicationClient::onNotification(Json::Value &jsonObj)
{
    std::string method = jsonObj["method"].asString();
    if(method == "BasicCommunication.SDLLog"){
//        int app_id=jsonObj["app_id"].asInt();
//        int correlation_id=jsonObj["correlation_id"].asInt();
//        std::string function=jsonObj["function"].asString();
//        Json::Value data=jsonObj["data"];
    }
    else{
        Channel::onNotification(jsonObj);
    }
}


