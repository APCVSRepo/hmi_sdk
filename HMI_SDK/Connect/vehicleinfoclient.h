#ifndef VEHICLEINFOCLIENT_H
#define VEHICLEINFOCLIENT_H

#include <json/json.h>

#include "ISocketManager.h"
#include "Channel.h"

class vehicleInfoClient : public Channel
{
public:
    vehicleInfoClient();
    ~vehicleInfoClient();

protected:
    void onRequest(Json::Value);

private:
    void subscribeVehicleDataResponse(int id);
    void unSubscribeVehicleDataResponse(int id);
    void getVehicleTypeResponse(int id);
    void isReady(int id);
    void getVehicleData(Json::Value);
    void sendGetVehicleDataResut(int, Json::Value);
    void sendGetVehicleDataError(int, Json::Value);
    void vehicleInfoGetDTCsResponse(Json::Value);
    void vehicleInfoReadDIDResponse(Json::Value);

};

#endif // VEHICLEINFOCLIENT_H
