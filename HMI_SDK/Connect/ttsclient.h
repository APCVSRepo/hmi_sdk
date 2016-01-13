#ifndef TTSCLIENT_H
#define TTSCLIENT_H

#include "json/json.h"

#include "ISocketManager.h"
#include "Channel.h"

class ttsClient : public Channel
{
public:
    ttsClient();
    ~ttsClient();
protected:
    void onRequest(Json::Value &);

private:

};

#endif // TTSCLIENT_H
