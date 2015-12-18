#include "Include/global_first.h"

#ifdef WINCE
#else
#include <sys/stat.h>
#endif

#include <iostream>
#include <stdio.h>
#include <string>
#include <json/json.h>
#include <fstream>
#include <cassert>
#include<Connect/Channel.h>
#ifdef WIN32
#ifdef WINCE
#include "unistd.h"
#else
#include <direct.h>
#endif
#else
//#include <unistd.h>
#endif
#include<stdlib.h>
#include "Config/Config.h"

JsonBuffer::JsonBuffer()
{
    m_szBuffer = "";
}

bool JsonBuffer::getJsonFromBuffer(char * pData, int iLength, Json::Value& output)
{
    if (pData != 0 && iLength > 0)
        m_szBuffer.append(pData, iLength);
    if (m_szBuffer.length() < 10)
        return false;

    int pos = m_szBuffer.find_first_of('\n');
    if (int(std::string::npos) == pos)
        return false;

    std::string szFirstJson = m_szBuffer.substr(0, pos + 1);
    if ((pos + 1) >= m_szBuffer.length())
        m_szBuffer = "";
    else
        m_szBuffer = m_szBuffer.substr(pos + 1);

    Json::Reader reader;
    if (reader.parse(szFirstJson, output))
    {
        return true;

    }
    return false;
}


Channel::Channel(std::string Channelname)
{
    m_iIDStart = -1;
    m_iRequestId = -1;

    m_iAppID = -1;

    m_iRegisterRequestId = -1;
    m_iUnregisterRequestId = -1;
    m_iIDRange = 1000;

    m_sComponentName = Channelname;

    ReadConfigure();
}

Channel::~Channel()
{

}

void Channel::ReadConfigure()
{
    // init staticConfigDB
    std::ifstream ifs;
    char szPath[1024];
#ifdef WIN32
#ifdef WINCE
    wchar_t wszPath[1024];
    GetModuleFileName( NULL, wszPath, MAX_PATH );
    wchar_t *lpwszSplit = wcsrchr(wszPath, '\\');
    *lpwszSplit = 0;
    int nLength = WideCharToMultiByte(CP_ACP, 0, wszPath, -1, NULL, 0, NULL, NULL);
    memset(szPath, 0, sizeof(szPath));
    WideCharToMultiByte(CP_ACP, 0, wszPath, -1, szPath, nLength, NULL, NULL);
#else
    _getcwd(szPath,1024);
#endif
#elif ANDROID
    sprintf(szPath,"%s",CONFIG_DIR);
#else
    getcwd(szPath,1024);
#endif
    char szDB[1024]={0};
#ifdef WIN32
    ::sprintf(szDB, "%s\\%s", szPath, "Config\\staticConfigDB.json");//..\\hmi-sdk-plus\\HMISDK\\
     LOGD("szDB=%s\n",szDB);
#elif ANDROID
    ::sprintf(szDB, "%s/%s", szPath, "staticConfigDB.json");
     LOGD("szDB=%s\n",szDB);
#else
    ::sprintf(szDB, "%s/%s", szPath, "Config/staticConfigDB.json");
     LOGD("szDB=%s\n",szDB);
#endif

    std::cout << szDB <<"\n";
    std::cout<<szPath<<"\n";
    ifs.open(szDB);
    assert(ifs.is_open());

    Json::Reader reader;

    if (!reader.parse(ifs, m_StaticConfigJson, false))
    {
        std::cout << "staticConfigJson error.\n";
    }
    else
    {
        m_ResultCodeJson = m_StaticConfigJson["resultCode"];
    }
    ifs.close();

}

void Channel::unRegisterComponent()
{
    m_iRegisterRequestId = m_iIDStart;
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iRegisterRequestId;
    root["method"] = "MB.unregisterComponent";

    params["componentName"] = m_sComponentName;

    root["params"] = params;
    SendJson(root);
}

void Channel::SetCallback(IMessageInterface * pCallback)
{
    m_pCallback = pCallback;
}

void Channel::setSocketManager(ISocketManager * pSocketManager, void * pHandle)
{
    m_pSocketManager = pSocketManager;
    m_pHandle = pHandle;
}

std::string	Channel::getChannelName()
{
    return m_sComponentName;
}

void Channel::onReceiveData(void * pData, int iLength)
{
    Json::Value RPC;
    while (m_JsonBuffer.getJsonFromBuffer((char*)pData, iLength, RPC))
    {
        onMessage(RPC);
        pData = 0;
        iLength = 0;
    }
}

void Channel::sendError(int resultCode, int id, std::string method, std::string message){
    Json::Value root;
    Json::Value error;
    Json::Value data;

    data["method"] = method;
    error["code"] = resultCode;
    error["data"] = data;
    error["message"] = message;

    root["jsonrpc"] = "2.0";
    root["id"] = id;
    root["error"] = error;

    SendJson(root);
}

void Channel::SendJson(Json::Value data)
{
    Json::FastWriter writer;
    std::string json_file = writer.write(data);
    const char * pStr = json_file.c_str();
    m_pSocketManager->SendData(m_pHandle, (void *)pStr, json_file.length());
}

void Channel::onMessage(Json::Value jsonObj)
{
    LOGI("onMessage:%s",jsonObj.toStyledString().data());
    std::string _methon = "";
    if (jsonObj.isMember("method") && jsonObj["method"].asString() == "BasicCommunication.SDLLog")
    {
        _methon = jsonObj["method"].asString();
    }

    if (jsonObj.isMember("params"))
    {
        if (jsonObj["params"].isMember("appID"))
        {
            m_iAppID = jsonObj["params"]["appID"].asInt();
        }
    }
    // id
    if (jsonObj.isMember("id"))
    {
        if (jsonObj["id"].asInt() == m_iRegisterRequestId)
        {
            if (!jsonObj.isMember("error"))
            {
                if (jsonObj.isMember("result"))
                {
                    m_iRequestId = jsonObj["result"].asInt();
                    m_iIDStart = jsonObj["result"].asInt();
                    onRegistered();
                }
            }
        }
        // handle component unregistration
        else if (jsonObj["id"].asInt() == m_iUnregisterRequestId)
        {
            if (!jsonObj.isMember("error"))
            {
                onUnregistered();
            }
        }
        // handle result, error, notification, requests
        else
        {
            if (jsonObj.isMember("result"))
            {
                onResult(jsonObj);
            }
            else if (jsonObj.isMember("error"))
            {
                onError(jsonObj["error"].asString());
            }
            else
            {
                onRequest(jsonObj);
            }
        }
    }
    else
    {
        onNotification(jsonObj);
    }
}

void Channel::onOpen()
{
    m_iRegisterRequestId = m_iIDStart;

    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iIDStart;
    root["method"] = "MB.registerComponent";

    params["componentName"] = m_sComponentName;

    root["params"] = params;
    SendJson(root);
}

void Channel::onRegistered()
{
}

void Channel::onUnregistered()
{

}

void Channel::GenerateId()
{
    m_iRequestId = m_iRequestId + 1;
    if (m_iRequestId >= m_iIDStart + m_iIDRange)
    {
        m_iRequestId = m_iIDStart;
    }
}

void Channel::onRequest(Json::Value data)
{
    m_pCallback->onRequest(data);
}

void Channel::onNotification(Json::Value data)
{
    m_pCallback->onNotification(data);
}
void Channel::onResult(Json::Value data)
{
    m_pCallback->onResult(data);
}
void Channel::onRawData(void * p, int iLength)
{
    m_pCallback->onRawData(p, iLength);
}

void Channel::onError(std::string error)
{
    m_pCallback->onError(error);
}

void Channel::SubscribeToNotification(std::string notification)
{
    GenerateId();
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iRequestId;
    root["method"] = "MB.subscribeTo";

    params["propertyName"] = notification;

    root["params"] = params;
    SendJson(root);
}

void Channel::UnsubscribeFromNotification(std::string notification)
{
    GenerateId();
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iRequestId;
    root["method"] = "MB.unsubscribeTo";

    params["propertyName"] = notification;

    root["params"] = params;
    SendJson(root);
}
