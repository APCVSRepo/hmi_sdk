#include "DataManager.h"
#include "AppList.h"
AppListInterface* pListInterface;
AppDataInterface* pDataInterface;
int               gAppId;
DataManager::DataManager(UIInterface *uiInterface)
{
    AppList *applist=new AppList;
    applist->setUIManager(uiInterface);
    pListInterface=applist;
    pDataInterface=applist->getAppDataInterface();
}



void DataManager::start()
{
    ((AppList*)pListInterface)->start();
}

void DataManager::switchNewApp(int newAppID)
{
    pListInterface->OnApplicationOut(gAppId);

    gAppId = newAppID;
    pListInterface->OnAppActivated(gAppId);
}

void DataManager::setAppId(int appId)
{
    qDebug()<<appId<<"+++++";
    gAppId = appId;
    pListInterface->OnAppActivated(gAppId);
}

AppListInterface* DataManager::ListInterface()
{
    return pListInterface;
}
AppDataInterface* DataManager::DataInterface()
{return pDataInterface;}
int DataManager::AppId()
{return gAppId;}



DataManager::~DataManager()
{

}

