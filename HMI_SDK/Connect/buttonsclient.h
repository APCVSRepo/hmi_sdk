#ifndef BUTTONSCLIENT_H
#define BUTTONSCLIENT_H

#include "json/json.h"

#include "Connect/Channel.h"

class buttonsClient : public Channel
{
public:
    buttonsClient();
    ~buttonsClient();
public:
    void onRequest(Json::Value &);

private:
};

#endif // BUTTONSCLIENT_H
