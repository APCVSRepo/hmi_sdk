#ifndef BASECOMMUNICATIONCLIENT_H
#define BASECOMMUNICATIONCLIENT_H

#include <json/json.h>

#include "Connect/Channel.h"

class baseCommunicationClient : public Channel
{
public:
    baseCommunicationClient();
    ~baseCommunicationClient();

protected:
    void onRegistered();
    void onUnregistered();
    void onReady();

public:
    void onRequest(Json::Value);

private:
    void isReady(int id);
    void mixingAudioSupported();
    void allowAllApps();
    void allowApp();
    void allowDeviceToConnect(int id);
    void sendBCResult(int id, std::string method);
    void sendActiveteApp(int id);
    void sendSystemInfo(int id);

};

#endif // BASECOMMUNICATIONCLIENT_H

