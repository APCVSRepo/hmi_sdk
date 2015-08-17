#include "HMISDK/include/global_first.h"
#include "HMISDK/include/connect/basecommunicationclient.h"

#include <iostream>
#include <string>
#include "HMISDK/include/json/json.h"

baseCommunicationClient::baseCommunicationClient() : Channel("BasicCommunication")
{
    m_iIDStart = 600;
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
    onReady();
}

void baseCommunicationClient::onUnregistered()
{
	UnsubscribeFromNotification("BasicCommunication.OnAppRegistered");
	UnsubscribeFromNotification("BasicCommunication.OnAppUnregistered");
	UnsubscribeFromNotification("BasicCommunication.PlayTone");
	UnsubscribeFromNotification("BasicCommunication.SDLLog");
}

void baseCommunicationClient::onReady()
{
    Json::Value root;

    root["jsonrpc"] = "2.0";
    root["method"] = "BasicCommunication.OnReady";

    SendJson(root);
}

void baseCommunicationClient::onRequest(Json::Value request)
{
    std::string method = request["method"].asString();
    
    if (method == "BasicCommunication.MixingAudioSupported")
    {
        mixingAudioSupported();
    }
    else if (method == "BasicCommunication.AllowAllApps")
    {
        allowAllApps();
    }
    else if (method == "BasicCommunication.AllowApp")
    {
        allowApp();
    }
    else if (method == "BasicCommunication.AllowDeviceToConnect")
    {
        allowDeviceToConnect(request["id"].asInt());
    }
    else if (method == "BasicCommunication.UpdateAppList")
    {
        sendBCResult(request["id"].asInt(), method);
    }
    else if (method == "BasicCommunication.UpdateDeviceList")
    {
        sendBCResult(request["id"].asInt(), method);
    }
    else if (method == "BasicCommunication.ActivateApp")
    {
        sendActiveteApp(request["id"].asInt());
    }
    else if (method == "BasicCommunication.IsReady")
    {
        isReady(request["id"].asInt());
    }
    else
    {
		m_pCallback->onRequest(request);
    }
}

void baseCommunicationClient::isReady(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "BasicCommunication.IsReady";
    result["available"] = true;

    root["result"] = result;
    SendJson(root);
}

void baseCommunicationClient::mixingAudioSupported()
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iIDStart;

    result["code"] = 0;
    result["method"] = "BasicCommunication.MixingAudioSupported";
    result["attenuatedSupported"] = true;

    root["result"] = result;
	SendJson(root);
}


void baseCommunicationClient::allowAllApps()
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iIDStart;

    result["code"] = 0;
    result["method"] = "BasicCommunication.AllowAllApps";
    result["allowed"] = true;

    root["result"] = result;
	SendJson(root);
}


void baseCommunicationClient::allowApp()
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iIDStart;

    result["code"] = 0;
    result["method"] = "BasicCommunication.AllowApp";
    result["allowed"] = true;

    root["result"] = result;
	SendJson(root);
}

void baseCommunicationClient::allowDeviceToConnect(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "BasicCommunication.AllowDeviceToConnect";
    result["allow"] = true;

    root["result"] = result;
	SendJson(root);
}



void baseCommunicationClient::sendBCResult(int id, std::string method)
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

void baseCommunicationClient::sendActiveteApp(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "BasicCommunication.ActivateApp";

    root["result"] = result;
	SendJson(root);
}
