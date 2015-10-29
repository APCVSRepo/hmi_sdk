#include "Include/global_first.h"
#include "AppList.h"

AppList::AppList()
{
}

void AppList::start()
{
    SDLConnector::getSDLConnectore()->ConnectToSDL(this);
}

void AppList::setUIManager(UIInterface *pUIManager)
{
    m_pUIManager = pUIManager;
    m_AppData.setUIManager(pUIManager);
}

AppDataInterface* AppList::getAppDataInterface()
{
    return &m_AppData;
}

int AppList::getCurrentAppID()
{
    return m_i_currentAppID;
}

void AppList::onRequest(Json::Value jsonObj)
{
    recvFromServer(jsonObj);
}

void AppList::onNotification(Json::Value jsonObj)
{
    if (jsonObj.isMember("method") && jsonObj["method"].asString() != "BasicCommunication.SDLLog"){
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
            std::cout << "+++++recvFromServer+++++\n";
            std::cout << jsonObj.toStyledString() << std::endl;
            std::cout << "---------\n";

            newAppRegistered(jsonObj);
            m_pUIManager->onAppShow(ID_APPLINK);
        }
        else if (str_method == "BasicCommunication.OnAppUnregistered")
        {
            std::cout << "+++++recvFromServer+++++\n";
            std::cout << jsonObj.toStyledString() << std::endl;
            std::cout << "---------\n";

            appUnregistered(jsonObj);
            m_pUIManager->onAppShow(ID_APPLINK);
        }
        else
        {
            m_AppData.recvFromServer(jsonObj);
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
    m_vec_json_newApp.push_back(jsonObj);
}

void AppList::OnAppActivated(int appID)
{
    m_i_currentAppID = appID;
    m_AppData.setCurrentAppID(m_i_currentAppID);
    for(int i = 0; i < m_vec_json_newApp.size(); i++)
    {
        if(m_i_currentAppID == m_vec_json_newApp.at(i)["params"]["application"]["appID"].asInt())
        {
            m_AppData.setCurrentAppName(m_vec_json_newApp.at(i)["params"]["application"]["appName"].asString());
        }
    }

    SDLConnector::getSDLConnectore()->OnAppActivated(appID);
}

void AppList::OnApplicationOut(int appID)
{
    SDLConnector::getSDLConnectore()->OnApplicationExit(appID);
}

void AppList::OnApplicationExit(int appID)
{
    SDLConnector::getSDLConnectore()->OnApplicationOut(appID);
}

std::vector <Json::Value > AppList::getNewAppJsonVector()
{
    return m_vec_json_newApp;
}

void AppList::appUnregistered(Json::Value jsonObj)
{
    int appID = jsonObj["params"]["appID"].asInt();

    for(int i = 0; i < m_vec_json_newApp.size(); i++)
    {
        if(appID == m_vec_json_newApp.at(i)["params"]["application"]["appID"].asInt())
        {
            m_vec_json_newApp.erase(m_vec_json_newApp.begin()+i);
        }
    }
}
