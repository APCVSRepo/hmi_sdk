//#include "HMISDK/include/global_first.h"
#include <Connect/navigationclient.h>

#include <iostream>
#include <string>
#include "json/json.h"

navigationClient::navigationClient() : Channel(800,"Navigation")
{

}

navigationClient::~navigationClient()
{

}


void navigationClient::onRequest(Json::Value &request)
{
    std::string method = request["method"].asString();
    int  id = request["id"].asInt();
    if (method == "Navigation.IsReady")
    {
        sendResult(id,"IsReady");
    }
    else if (method == "Navigation.ShowConstantTBT")
    {
        sendResult(id,"ShowConstantTBT");
    }
    else if (method == "Navigation.UpdateTurnList")
    {
        sendResult(id,"UpdateTurnList");
    }
    else if (method == "Navigation.AlertManeuver")
    {
        sendResult(id,"AlertManeuver");
    }
    else if (method == "Navigation.StartStream")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"StartStream",result);
    }
    else if (method == "Navigation.StopStream")
    {
        Result result=m_pCallback->onRequest(request);
        sendResult(id,"StopStream",result);
    }
    else{
        Channel::onRequest(request);
    }
}
