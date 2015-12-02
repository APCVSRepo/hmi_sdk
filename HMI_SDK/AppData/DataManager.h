#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "AppListInterface.h"
#include "UIInterface.h"
class DataManager
{
public:
    DataManager(UIInterface *uiInterface=NULL);
    ~DataManager();
    void start();
    static void switchNewApp(int newAppID);
    static void setAppId(int appId);
    static AppListInterface* ListInterface();
    static AppDataInterface* DataInterface();
    static int AppId();
private:

};

#endif // DATAMANAGER_H
