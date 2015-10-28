#ifndef BUTTONSCLIENT_H
#define BUTTONSCLIENT_H

#include "json/json.h"

#include "connect/Channel.h"

class buttonsClient : public Channel
{
public:
    buttonsClient();
    ~buttonsClient();

public:
    void onRequest(Json::Value);

private:
    void getCapabilities(int id);
    void isReady(int id);
};

#endif // BUTTONSCLIENT_H
