#ifndef APPLISTINTERFACE_H
#define APPLISTINTERFACE_H

#include "json/json.h"
#include <iostream>
#include <string.h>
#include <vector>
#include "AppDataInterface.h"

class AppListInterface
{
public:
    virtual std::vector <Json::Value > getNewAppJsonVector() = 0;
    virtual void OnAppActivated(int appID) = 0;
    virtual void OnApplicationOut(int appID) = 0;
    virtual void OnApplicationExit(int appID) = 0;
    virtual AppDataInterface* getAppDataInterface() = 0;
};

#endif // APPLISTINTERFACE_H
