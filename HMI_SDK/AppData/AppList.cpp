#include "Include/global_first.h"
#include "AppList.h"

extern std::string string_To_UTF8(const std::string & str);
extern bool IsTextUTF8(char* str, unsigned long long length);


AppList::AppList()
{
    m_pCurApp = NULL;
}

AppList::~AppList()
{
    int i;
    for(i = 0; i < m_AppDatas.size(); i++)
    {
        delete m_AppDatas[i];
    }
}

void AppList::setUIManager(UIInterface *pUIManager)
{
    m_pUIManager = pUIManager;
    if(m_pCurApp)
        m_pCurApp->setUIManager(pUIManager);
}

void AppList::ShowAppList()
{
    m_pUIManager->onAppShow(ID_APPLINK);
}

AppDataInterface* AppList::getActiveApp()
{
    return m_pCurApp;
}

void AppList::onRequest(Json::Value jsonObj)
{
    recvFromServer(jsonObj);
}

void AppList::onNotification(Json::Value jsonObj)
{
    //
    if (jsonObj.isMember("method") && jsonObj["method"].asString() != "BasicCommunication.SDLLog")
    {
        recvFromServer(jsonObj);
    }
}

void AppList::onResult(Json::Value jsonObj)
{
    recvFromServer(jsonObj);
}

void AppList::onRawData(void*, int)
{
}

void AppList::onError(std::string)
{
}

void AppList::recvFromServer(Json::Value jsonObj)
{
    if(jsonObj.isMember("method"))
    {
        std::string str_method = jsonObj["method"].asString();

        if (str_method == "BasicCommunication.OnAppRegistered")
        {
            newAppRegistered(jsonObj);
            m_pUIManager->onAppShow(ID_APPLINK);
        }
        else if (str_method == "BasicCommunication.OnAppUnregistered")
        {
            appUnregistered(jsonObj);
            m_pUIManager->onAppShow(ID_APPLINK);
        }
        else if (str_method == "VR.VRExitApp")
        {
            m_pUIManager->tsSpeak(ID_EXIT, "退出"+ m_pCurApp->m_szAppName);
            m_pUIManager->onAppShow(ID_APPLINK);
        }
        else if(str_method == "VR.VRSwitchApp")
        {
//            {
//               "jsonrpc" : "2.0",
//               "method" : "VR.VRSwitchApp",
//               "params" : {
//                  "appID" : 18467,
//                  "appVRName" : "百度 "
//               }
//            }
            std::string strAppVRName = jsonObj["params"]["appVRName"].asString();
            if(!IsTextUTF8((char *)strAppVRName.data(),strAppVRName.size()))
                strAppVRName = string_To_UTF8(strAppVRName);

            m_pUIManager->tsSpeak(ID_SWITCHAPP, strAppVRName);

            int iNewID = jsonObj["params"]["appID"].asInt();
            if(m_pCurApp->m_iAppID != iNewID)
            {
                std::vector <AppData *>::iterator i;
                for(i = m_AppDatas.begin(); i != m_AppDatas.end(); i++)
                {
                    if(iNewID == (*i)->m_iAppID)
                    {
                        m_pCurApp = *i;
                        m_pUIManager->onAppShow(m_pCurApp->getCurUI());
                        break;
                    }
                }
            }
        }
        else
        {
            if(m_pCurApp)
                m_pCurApp->recvFromServer(jsonObj);
        }

    }
}


//{
//   "jsonrpc" : "2.0",
//   "method" : "BasicCommunication.OnAppRegistered",
//   "params" : {
//      "application" : {
//         "appID" : 846930886,
//         "appName" : "Sina Weibo",
//         "appType" : [ "NAVIGATION" ],
//         "deviceName" : "10.27.0.132",
//         "hmiDisplayLanguageDesired" : "EN-US",
//         "icon" : "",
//         "isMediaApplication" : true,
//         "ngnMediaScreenAppName" : "Sina "
//      },
//      "vrSynonyms" : [ "Sina Weibo" ]
//   }
//}
//{
//   "jsonrpc" : "2.0",
//   "method" : "BasicCommunication.OnAppRegistered",
//   "params" : {
//      "application" : {
//         "appID" : 783368690,
//         "appName" : "SyncProxyTester",
//         "appType" : [
//            "DEFAULT",
//            "COMMUNICATION",
//            "MEDIA",
//            "MESSAGING",
//            "NAVIGATION",
//            "INFORMATION",
//            "SOCIAL",
//            "BACKGROUND_PROCESS",
//            "TESTING",
//            "SYSTEM"
//         ],
//         "deviceName" : "10.27.0.132",
//         "hmiDisplayLanguageDesired" : "EN-US",
//         "icon" : "",
//         "isMediaApplication" : true,
//         "ngnMediaScreenAppName" : "SPT"
//      },
//      "resumeVrGrammars" : true,
//      "ttsName" : [
//         {
//            "text" : "TTS Name 1",
//            "type" : "TEXT"
//         },
//         {
//            "text" : "Tester",
//            "type" : "TEXT"
//         }
//      ],
//      "vrSynonyms" : [ "SPT", "Tester" ]
//   }
//}
void AppList::newAppRegistered(Json::Value jsonObj)
{
    AppData * pData = new AppData();
    pData->setUIManager(m_pUIManager);
    pData->m_iAppID = jsonObj["params"]["application"]["appID"].asInt();
    pData->m_szAppName = jsonObj["params"]["application"]["appName"].asString();
    pData->addExitAppCommand();
    m_AppDatas.push_back(pData);
}

void AppList::OnAppActivated(int iAppID)
{
    AppData * pData;
    int i;
    for(i = 0; i < m_AppDatas.size(); i++)
    {
        pData = m_AppDatas[i];
        if(pData->m_iAppID == iAppID)
            break;
    }

    if(i >= m_AppDatas.size())  // not found
    {
        return;
    }

    if(m_pCurApp != NULL)
        SDLConnector::getSDLConnectore()->OnAppOut(m_pCurApp->m_iAppID);
    m_pCurApp = pData;
    SDLConnector::getSDLConnectore()->OnAppActivated(iAppID);
    m_pUIManager->onAppShow(ID_MAIN);
}

void AppList::OnAppExit()
{
    SDLConnector::getSDLConnectore()->OnAppExit(m_pCurApp->m_iAppID);
    m_pCurApp = NULL;
    m_pUIManager->onAppShow(ID_APPLINK);
}

void AppList::getAppList(std::vector<int>& vAppIDs, std::vector<std::string>& vAppNames)
{
    for(int i = 0; i < m_AppDatas.size(); i++)
    {
        vAppIDs.push_back(m_AppDatas[i]->m_iAppID);
        vAppNames.push_back(m_AppDatas[i]->m_szAppName);
    }
}

void AppList::appUnregistered(Json::Value jsonObj)
{
    int appID = jsonObj["params"]["appID"].asInt();

    std::vector <AppData *>::iterator i;
    for(i = m_AppDatas.begin(); i != m_AppDatas.end(); i++)
    {
        if(appID == (*i)->m_iAppID)
        {
            delete *i;
            m_AppDatas.erase(i);
            break;
        }
    }
}
