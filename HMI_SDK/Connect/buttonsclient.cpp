#include "Include/global_first.h"
#include "Connect/buttonsclient.h"

#include <iostream>
#include <string>
#include "json/json.h"

buttonsClient::buttonsClient() : Channel(200,"Buttons")
{

}

buttonsClient::~buttonsClient()
{

}


void buttonsClient::onRequest(Json::Value &request)
{
    std::string method = request["method"].asString();
    int  id = request["id"].asInt();
    if (method == "Buttons.GetCapabilities")
    {
        sendResult(id,"GetCapabilities");
    }
    else if (method == "Buttons.IsReady")
    {
        sendResult(id,"IsReady");
    }
    else
    {
        Channel::onRequest(request);
    }
}

