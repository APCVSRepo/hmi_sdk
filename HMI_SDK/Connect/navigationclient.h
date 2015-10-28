#ifndef NAVIGATIONCLIENT_H
#define NAVIGATIONCLIENT_H


#include "json/json.h"
#include "ISocketManager.h"
#include "Channel.h"

class navigationClient : public Channel
{
public:
    navigationClient();
    ~navigationClient();

protected:
    void onRequest(Json::Value);

private:
    void sendNavigationResult(int id, std::string method);
    void isReady(int id);
};

#endif // NAVIGATIONCLIENT_H
