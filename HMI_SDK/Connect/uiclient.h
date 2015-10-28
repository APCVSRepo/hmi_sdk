﻿#ifndef UICLIENT_H
#define UICLIENT_H

#include <json/json.h>
#include "ISocketManager.h"
#include "Channel.h"


class uiClient : public Channel
{
public:
    uiClient();
    ~uiClient();

protected:
    void onRegistered();
    void onUnregistered();

    void onRequest(Json::Value);

private:
    void sendUIResult(int id, std::string method);
    void sendMediaClockTimerResponse(int id);
    void getSupportedLanguages(int id);
    void getLanguage(int id);
    void getCapabilities(int id);
    void isReady(int id);
    void closePopUp(int id);
    void startRecording(Json::Value);

public:
    void onSystemContext(std::string systemContext);
};

#endif // UICLIENT_H
