#ifndef APPLISTINTERFACE_H
#define APPLISTINTERFACE_H

#include "json/json.h"
#include <iostream>
#include <string.h>
#include <vector>
#include "AppDataInterface.h"
#include "UIInterface.h"

class AppListInterface
{
public:
    virtual void getAppList(std::vector<int>& vAppIDs, std::vector<std::string>& vAppNames) = 0;
    virtual void OnAppActivated(int appID) = 0;
    virtual void OnApplicationOut(int appID) = 0;
    virtual void OnApplicationExit() = 0;
    virtual AppDataInterface* getAppDataInterface() = 0;
    virtual void setUIManager(UIInterface *pUI) = 0;
};

#endif // APPLISTINTERFACE_H
