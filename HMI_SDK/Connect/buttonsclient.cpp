//#include "HMISDK/include/global_first.h"
#include "Connect/buttonsclient.h"

#include <iostream>
#include <string>
#include "json/json.h"

buttonsClient::buttonsClient() : Channel("Buttons")
{
    m_iIDStart = 200;
}

buttonsClient::~buttonsClient()
{

}

void buttonsClient::onRequest(Json::Value request)
{
    std::string method = request["method"].asString();

    if (method == "Buttons.GetCapabilities")
    {
        getCapabilities(request["id"].asInt());
    }
    else if (method == "Buttons.IsReady")
    {
        isReady(request["id"].asInt());
    }
    else
    {
		m_pCallback->onRequest(request);
    }
}

void buttonsClient::getCapabilities(int id)
{
    Json::Value root;
    Json::Value result;
    Json::Value presetBankCapabilities;

    presetBankCapabilities["presetBankCapabilities"] = true;

    result["capabilities"] = m_StaticConfigJson["capabilities"];
    result["code"] = 0;
    result["method"] = "Buttons.GetCapabilities";
    result["presetBankCapabilities"] = presetBankCapabilities;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    root["result"] = result;
	SendJson(root);
}

void buttonsClient::isReady(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "Buttons.IsReady";
    result["available"] = true;

    root["result"] = result;
	SendJson(root);
}
