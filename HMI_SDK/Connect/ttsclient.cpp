#include <global_first.h>
#include <connect/ttsclient.h>
#include <iostream>
#include <string>
#include "json/json.h"

ttsClient::ttsClient() : Channel("TTS")
{
    m_iIDStart = 300;
}

ttsClient::~ttsClient()
{

}

void ttsClient::onRequest(Json::Value request)
{
    std::string method = request["method"].asString();

    if(method == "TTS.SetGlobalProperties")
    {
        sendTTSResult(request["id"].asInt(), method);
    }
    else if(method == "TTS.GetCapabilities")
    {
        getCapabilities(request["id"].asInt());
    }
    else if(method == "TTS.GetSupportedLanguages")
    {
        getSupportedLanguages(request["id"].asInt());
    }
    else if(method == "TTS.GetLanguage")
    {
        getLanguage(request["id"].asInt());
    }
    else if(method == "TTS.ChangeRegistration")
    {
        sendTTSResult(request["id"].asInt(), method);
    }
    else if(method == "TTS.IsReady")
    {
        isReady(request["id"].asInt());
    }
    else
    {
        if (method == "TTS.Speak")
        {
            // ttsHandler action start
            // sendTTSResult(request["id"].asInt(), method);
        }
        else if(method == "TTS.StopSpeaking")
        {
            // ttsHandler action stop
            sendTTSResult(request["id"].asInt(), method);
        }
		m_pCallback->onRequest(request);
    }
}

void ttsClient::sendTTSResult(int id, std::string method)
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


void ttsClient::getCapabilities(int id)
{
    Json::Value root;
    Json::Value result;

    result["capabilities"] = m_StaticConfigJson["capabilities_tts"];
    result["code"] = 0;
    result["method"] = "TTS.GetCapabilities";

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    root["result"] = result;
	SendJson(root);
}

void ttsClient::getSupportedLanguages(int id)
{
    Json::Value root;
    Json::Value result;

    result["languages"] = m_StaticConfigJson["sdlLanguagesList"];
    result["code"] = 0;
    result["method"] = "TTS.GetSupportedLanguages";

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    root["result"] = result;
	SendJson(root);
}

void ttsClient::getLanguage(int id)
{
    Json::Value root;
    Json::Value result;

    result["languages"] = m_StaticConfigJson["hmiTTSVRLanguage"];
    result["code"] = 0;
    result["method"] = "TTS.GetLanguage";

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    root["result"] = result;
	SendJson(root);
}


void ttsClient::isReady(int id)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = id;

    result["code"] = 0;
    result["method"] = "TTS.IsReady";
    result["available"] = true;

    root["result"] = result;
	SendJson(root);
}
