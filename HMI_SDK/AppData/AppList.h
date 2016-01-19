#ifndef APPLIST_H
#define APPLIST_H

#include "Connect/connect.h"

#include "AppListInterface.h"
#include "AppData.h"
#include "UI/UIInterface.h"

class AppData;
class AppList : public IMessageInterface, public AppListInterface
{
public:
    AppList();
    ~AppList();

    //AppListInterface
    void setUIManager(UIInterface *pUI);
    AppDataInterface* getActiveApp();
    void getAppList(std::vector<int>& vAppIDs, std::vector<std::string>& vAppNames);
    void getAppList(std::vector<int>& vAppIDs, std::vector<std::string>& vAppNames,std::vector<std::string> &vIconPath);
    void OnAppActivated(int appID);
    void OnAppExit();
    void ShowPreviousUI();

public: //IMessageInterface
    Result onRequest(Json::Value);
    void onNotification(Json::Value);
    void onResult(Json::Value);
    void onRawData(void * p, int iLength);
    void onError(std::string error);

private:
    Result recvFromServer(Json::Value);
    void newAppRegistered(Json::Value jsonObj);
    void appUnregistered(Json::Value jsonObj);

private:
    UIInterface *m_pUIManager;
    std::vector <AppData *> m_AppDatas;
    AppData * m_pCurApp;
};

#endif // APPLIST_H
