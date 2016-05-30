﻿#ifndef APPLISTINTERFACE_H
#define APPLISTINTERFACE_H

#include "json/json.h"
#include <iostream>
#include <string.h>
#include <vector>
#include "AppDataInterface.h"
#include "UIInterface.h"
#include "../hmi_sdk/HMI_SDK/Connect/IMessageInterface.h"

class AppListInterface
{
public:
    virtual void OnAppActivated(int appID) = 0;
    virtual void OnAppExit() = 0;
    virtual void ShowPreviousUI() = 0;
    virtual AppDataInterface* getActiveApp() = 0;
    virtual void setUIManager(UIInterface *pUI) = 0;
    virtual void getAppList(std::vector<int>& vAppIDs, std::vector<std::string>& vAppNames) = 0;
    virtual void getAppList(std::vector<int>& vAppIDs, std::vector<std::string>& vAppNames,std::vector<std::string> &vIconPath) = 0;

    virtual void IconnectToVS(IMessageInterface * pMsgHandler, std::string sIP, int iPort) = 0;
    virtual void IdelConnectToVS() = 0;
};

#endif // APPLISTINTERFACE_H
