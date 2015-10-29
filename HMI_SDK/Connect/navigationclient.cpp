//#include "HMISDK/include/global_first.h"
#include <Connect/navigationclient.h>

#include <iostream>
#include <string>
#include "json/json.h"

navigationClient::navigationClient() : Channel("Navigation")
{
    m_iIDStart = 800;
}

navigationClient::~navigationClient()
{

}

void navigationClient::onRequest(Json::Value request)
{
    std::string method = request["method"].asString();

    if (method == "Navigation.IsReady")
    {
        isReady(request["id"].asInt());
    }
    else if (method == "Navigation.ShowConstantTBT")
    {
        sendNavigationResult(request["id"].asInt(), request["method"].asString());
    }
    else if (method == "Navigation.UpdateTurnList")
    {
        sendNavigationResult(request["id"].asInt(), request["method"].asString());
    }
    else if (method == "Navigation.AlertManeuver")
    {
        sendNavigationResult(request["id"].asInt(), request["method"].asString());
    }
    else
    {
        if (method == "Navigation.StartStream")
        {
            sendNavigationResult(request["id"].asInt(), request["method"].asString());
        }
        else if (method == "Navigation.StopStream")
        {
            sendNavigationResult(request["id"].asInt(), request["method"].asString());
        }
		m_pCallback->onRequest(request);
    }
}

void navigationClient::sendNavigationResult(int id, std::string method)
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

void navigationClient::isReady(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "Navigation.IsReady";
    result["available"] = true;

    root["result"] = result;
	SendJson(root);
}
