//#include <global_first.h>
#include <Connect/vehicleinfoclient.h>

#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include "json/json.h"

vehicleInfoClient::vehicleInfoClient() : Channel("VehicleInfo")
{
    m_iIDStart = 700;
}

vehicleInfoClient::~vehicleInfoClient()
{

}

void vehicleInfoClient::onRequest(Json::Value request)
{
    std::string method = request["method"].asString();

    if(method == "VehicleInfo.SubscribeVehicleData")
    {
        subscribeVehicleDataResponse(request["id"].asInt());
    }
    else if(method == "VehicleInfo.UnsubscribeVehicleData")
    {
        unSubscribeVehicleDataResponse(request["id"].asInt());
    }
    else if(method == "VehicleInfo.GetVehicleType")
    {
        getVehicleTypeResponse(request["id"].asInt());
    }
    else if(method == "VehicleInfo.IsReady")
    {
        isReady(request["id"].asInt());
    }
    else if(method == "VehicleInfo.GetVehicleData")
    {
        getVehicleData(request);
    }
    else if(method == "VehicleInfo.ReadDID")
    {
        vehicleInfoReadDIDResponse(request);
    }
    else if(method == "VehicleInfo.GetDTCs")
    {
        vehicleInfoGetDTCsResponse(request);
    }
    else
    {
        m_pCallback->onRequest(request);
    }
}

void vehicleInfoClient::subscribeVehicleDataResponse(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "VehicleInfo.SubscribeVehicleData";

    root["result"] = result;
    SendJson(root);
}

void vehicleInfoClient::unSubscribeVehicleDataResponse(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "VehicleInfo.UnsubscribeVehicleData";

    root["result"] = result;
    SendJson(root);
}

void vehicleInfoClient::getVehicleTypeResponse(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "VehicleInfo.GetVehicleType";
    result["vehicleType"] = m_StaticConfigJson["vehicleType"];

    root["result"] = result;
    SendJson(root);
}

void vehicleInfoClient::isReady(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "VehicleInfo.IsReady";
    result["available"] = true;

    root["result"] = result;
    SendJson(root);
}

void vehicleInfoClient::getVehicleData(Json::Value message)
{
    std::ifstream ifs;
    ifs.open("config/VehicleInfo.json");
    assert(ifs.is_open());

    Json::Reader reader;
    Json::Value VehicleInfoJsonObj;
    Json::Value vehicle;
    Json::Value data;
    Json::Value params;
    int id;
    bool result = true;

    params = message["params"];
    id = message["id"].asInt();

    if (!reader.parse(ifs, VehicleInfoJsonObj, false))
    {
        std::cout << "VehicleInfoDataJson error.\n";
    }
    else
    {
        vehicle = VehicleInfoJsonObj["vehicle"];
    }
    ifs.close();
    Json::Value::Members mem = message.getMemberNames();
    for (Json::Value::Members::iterator iter = mem.begin(); iter != mem.end(); iter++)
    {
        if(std::string(*iter) != "appID" && std::string(*iter) != "request"){
            if(message[*iter].type() != Json::nullValue){
                if(vehicle.isMember(*iter)){
                    data[*iter] = vehicle[*iter];
                }else{
                    result = false;
                }
            }
        }
    }
    if(result){
        sendGetVehicleDataResut(id, data);
    }else{
        sendGetVehicleDataError(id, data);
    }
}

void vehicleInfoClient::sendGetVehicleDataError(int id, Json::Value data)
{
    Json::Value root;
    Json::Value error;

    root["id"] = id;
    root["jsonrpc"] = "2.0";

    error["message"] = "Params rpc, are not avaliable";
    error["code"] = 9;
    error["date"] = data;
    error["date"]["method"] = "VehicleInfo.GetVehicleData";

    root["error"] = error;
    SendJson(root);
}

void vehicleInfoClient::sendGetVehicleDataResut(int id, Json::Value data)
{
    Json::Value root;
    Json::Value result;

    root["id"] = id;
    root["jsonrpc"] = "2.0";

    result["code"] = 0;
    result["date"] = data;
    result["method"] = "VehicleInfo.GetVehicleData";

    root["error"] = result;
    SendJson(root);
}

void vehicleInfoClient::vehicleInfoReadDIDResponse(Json::Value request)
{
    std::ifstream ifs;
    ifs.open("config/VehicleInfo.json");
    assert(ifs.is_open());

    Json::Reader reader;
    Json::Value VehicleInfoJsonObj;
    Json::Value did;
    Json::Value didLocation;
    didLocation = request["params"]["didLocation"];
    int id;
    id = request["id"].asInt();

    if (!reader.parse(ifs, VehicleInfoJsonObj, false))
    {
        std::cout << "VehicleInfoDataJson error.\n";
    }
    else
    {
        did = VehicleInfoJsonObj["did"];
    }
    ifs.close();
    int size = int(didLocation.size());

    Json::Value arrayObj;
    Json::Value item;

    for (int i = 0; i < size; i++)
    {
        item["data"] = did[0];
        item["didLocation"] = didLocation[i];
        item["resultCode"] = "SUCCESS";
        arrayObj.append(item);
    }

    Json::Value root;
    Json::Value result;
    root["id"] = id;
    root["jsonrpc"] = "2.0";
    result["code"] = 0;
    result["method"] = "VehicleInfo.ReadDID";
    result["didResult"] = arrayObj;
    root["result"] = result;
    SendJson(root);
}

void vehicleInfoClient::vehicleInfoGetDTCsResponse(Json::Value request)
{
    std::ifstream ifs;
    ifs.open("config/VehicleInfo.json");
    assert(ifs.is_open());

    Json::Reader reader;
    Json::Value VehicleInfoJsonObj;
    Json::Value dtc;
    int id;
    id = request["id"].asInt();

    if (!reader.parse(ifs, VehicleInfoJsonObj, false))
    {
        std::cout << "VehicleInfoDataJson error.\n";
    }
    else
    {
        dtc = VehicleInfoJsonObj["dtc"];
    }
    ifs.close();

    Json::Value root;
    Json::Value result;
    root["id"] = id;
    root["jsonrpc"] = "2.0";
    result["code"] = 0;
    result["ecuHeader"] = 2;
    result["method"] = "VehicleInfo.GetDTCs";
    result["dtc"] = dtc;
    root["result"] = result;
    SendJson(root);
}
