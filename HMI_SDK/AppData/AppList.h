#ifndef APPLIST_H
#define APPLIST_H


#include "connect/connect.h"
#include "connect/SDLConnector.h"

#include "AppData/AppListInterface.h"
#include "AppData/AppData.h"
#include "UI/UIInterface.h"

class AppList : public IMessageInterface, public AppListInterface
{
public:
    AppList();

    void start();
    void setUIManager(UIInterface *pcallBack);

    AppDataInterface* getAppDataInterface();

    void OnAppActivated(int appID);
    void OnApplicationOut(int appID);
    void OnApplicationExit(int appID);

    std::vector <Json::Value > getNewAppJsonVector();
    int getCurrentAppID();


private:
    void onRequest(Json::Value);
    void onNotification(Json::Value);
    void onResult(Json::Value);
    void onRawData(void * p, int iLength);
    void onError(std::string error);
    void recvFromServer(Json::Value);

    std::vector <Json::Value > m_vec_json_newApp;
    void newAppRegistered(Json::Value jsonObj);
    void appUnregistered(Json::Value jsonObj);

    UIInterface *m_pUIManager;
    AppData m_AppData;
    int m_i_currentAppID;
};

#endif // APPLIST_H
