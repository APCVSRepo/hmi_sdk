#ifndef TTSCLIENT_H
#define TTSCLIENT_H

#include "HMISDK/include/json/json.h"

#include "ISocketManager.h"
#include "Channel.h"

class ttsClient : public Channel
{
public:
    ttsClient();
    ~ttsClient();

protected:
    void onRequest(Json::Value);

private:
    void sendTTSResult(int id, std::string method);
    void getCapabilities(int id);
    void getSupportedLanguages(int id);
    void getLanguage(int id);
    void isReady(int id);

};

#endif // TTSCLIENT_H
