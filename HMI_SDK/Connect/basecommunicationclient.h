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

public:
    void onRequest(Json::Value &);
    void onNotification(Json::Value &);

private:

};

#endif // BASECOMMUNICATIONCLIENT_H

