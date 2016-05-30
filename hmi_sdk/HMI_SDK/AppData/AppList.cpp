#include "global_first.h"
#include "AppList.h"
#include <QUrl>

extern std::string string_To_UTF8(const std::string & str);
extern bool IsTextUTF8(char* str, unsigned long long length);

std::string ChangeSlash(std::string strSrc)
{
#if defined(WIN32) || defined(WINCE)
    char *pTemp = new char[strSrc.size()+1];
    strcpy(pTemp,strSrc.c_str());
    for(int i = 0;i != strSrc.size();++i)
    {
        if(pTemp[i] == '\\')
        {
            pTemp[i] = '/';
        }
    }
    strSrc = pTemp;
    delete []pTemp;
#endif
    return strSrc;
}

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

void AppList::ShowPreviousUI()
{
    if(m_pCurApp)
    {
        if(m_pCurApp->ShowPreviousUI(false))
            return;
    }
    m_pUIManager->onAppShow(ID_APPLINK);
}

AppDataInterface* AppList::getActiveApp()
{
    return m_pCurApp;
}

Result AppList::onRequest(Json::Value jsonObj)
{
    return recvFromServer(jsonObj);
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

Result AppList::recvFromServer(Json::Value jsonObj)
{
    if(jsonObj.isMember("method"))
    {
       // LOGI(jsonObj.toStyledString().data());
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
        else if(str_method == "Navigation.StopStream")
        {
            m_pUIManager->onVideoStreamStop();
            ShowPreviousUI();
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
        else if(str_method == "UI.SetAppIcon")
        {
            int iAppId = jsonObj["params"]["appID"].asInt();
            std::vector <AppData *>::iterator Iter = m_AppDatas.begin();
            while(Iter != m_AppDatas.end())
            {
                if(iAppId == (*Iter)->m_iAppID)
                {
                    QUrl iconPathUrl(jsonObj["params"]["syncFileName"]["value"].asString().c_str());
#if defined(WINCE)
                    (*Iter)->m_strAppIconFilePath = ChangeSlash(iconPathUrl.path().toLocal8Bit().data());
#else
                    (*Iter)->m_strAppIconFilePath = ChangeSlash(iconPathUrl.path().toStdString());
#endif
                    if(m_pCurApp == NULL)
                    {
                        m_pUIManager->onAppShow(ID_APPLINK);
                    }
                    //m_pUIManager->onAppShow(m_pCurApp->getCurUI());
                    break;
                }
                ++Iter;
            }
        }        
        else if(str_method == "UI.EndAudioPassThru")
        {
            ToSDL->OnVRCancelRecord();
            m_pUIManager->OnEndAudioPassThru();
            return RESULT_SUCCESS;
        }
        else
        {
            if(m_pCurApp)
                return m_pCurApp->recvFromServer(jsonObj);
            else
                return RESULT_APPLICATION_NOT_REGISTERED;
        }
        return  RESULT_SUCCESS;
    }
    return  RESULT_SUCCESS;
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

    std::vector <AppData *>::iterator i;
    for(i = m_AppDatas.begin(); i != m_AppDatas.end(); i++)
    {
        AppData * pOne = *i;
        if(pOne->m_iAppID == pData->m_iAppID)
        {
            m_AppDatas.erase(i);
            delete pOne;
            break;
        }
    }

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
        ToSDL->OnAppOut(m_pCurApp->m_iAppID);
    m_pCurApp = pData;
    ToSDL->OnAppActivated(iAppID);
    m_pUIManager->onAppShow(m_pCurApp->getCurUI());
    m_pUIManager->onAppActive();
}

void AppList::OnAppExit()
{
    ToSDL->OnAppExit(m_pCurApp->m_iAppID);
    m_pCurApp = NULL;
    m_pUIManager->onAppShow(ID_APPLINK);
    m_pUIManager->onAppStop();
}

void AppList::getAppList(std::vector<int>& vAppIDs, std::vector<std::string>& vAppNames,std::vector<std::string> &vIconPath)
{
    for(int i = 0; i < m_AppDatas.size(); i++)
    {
        vAppIDs.push_back(m_AppDatas[i]->m_iAppID);
        vAppNames.push_back(m_AppDatas[i]->m_szAppName);
        vIconPath.push_back(m_AppDatas[i]->m_strAppIconFilePath);
        LOGI("---%s",m_AppDatas[i]->m_strAppIconFilePath.c_str());
    }
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
            if(m_pCurApp)
            {
                if(m_pCurApp->m_iAppID == appID)
                {
                    m_pCurApp = NULL;
                    //m_pUIManager->onVideoStreamStop();
                }
            }
            delete *i;
            m_AppDatas.erase(i);
            break;
        }
    }
}

void AppList::IconnectToVS(IMessageInterface * pMsgHandler, std::string sIP, int iPort)
{
    LOGI("IconnectToVS");
    ToSDL->ConnectToVideoStream(pMsgHandler,sIP,iPort);
}
void AppList::IdelConnectToVS()
{
    LOGI("IdelConnectToVS");
    ToSDL->DelConnectToVideoStream();
}
