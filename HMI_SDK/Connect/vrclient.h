#ifndef VRCLIENT_H
#define VRCLIENT_H

#include "ISocketManager.h"
#include "Channel.h"

class vrClient : public Channel
{
public:
    vrClient();
    ~vrClient();

protected:
    void onRegistered();
    void onUnregistered();

    void onRequest(Json::Value);

private:
    void sendVRResult(int code, int id, std::string method);
    void getSupportedLanguagesAction(int id);
    void getLanguage(int id);
    void isReady(int id);

};

#endif // VRCLIENT_H
