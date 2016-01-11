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

    void onRequest(Json::Value &);

private:

};

#endif // VRCLIENT_H
