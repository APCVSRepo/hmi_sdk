#include "Include/global_first.h"
#include <Connect/SDLConnector.h>
#include <vector>
#include <iostream>
#include <pthread.h>
#include "Config/Config.h"


static SDLConnector * g_SingleConnector = 0;

SDLConnector::SDLConnector() : m_bReleased(false), m_Sockets(), m_VR(), m_Base(), m_Buttons(), m_Navi(), m_TTS(), m_Vehicle(), m_UI()
{
    m_sdl_is_connected=false;
}

SDLConnector::~SDLConnector()
{
    g_SingleConnector =  0;
    m_bReleased = true;
}


SDLConnector * SDLConnector::getSDLConnectore()
{
    if (g_SingleConnector == 0)
        g_SingleConnector = new SDLConnector();
    return g_SingleConnector;
}

void SDLConnector::onConnected()
{
}

void SDLConnector::onNetworkBroken()
{
    if(m_pNetwork)
        m_pNetwork->onNetworkBroken();

    while(!m_bReleased)
    {
#if defined(WIN32)|| defined(WINCE)
        Sleep(1000);
#else
        usleep(1000000);
#endif
        if(!ConnectToSDL(m_pMsgHandler,m_pNetwork))
        {
            if(m_pNetwork)
                m_pNetwork->onConnected();
            break;
        }
    }
}

bool SDLConnector::IsSDLConnected()
{
    return m_sdl_is_connected;
}

bool SDLConnector::ConnectToSDL(IMessageInterface * pMsgHandler, INetworkStatus * pNetwork)
{
    m_pNetwork = pNetwork;
    m_pMsgHandler = pMsgHandler;

    ChangeMsgHandler(pMsgHandler);
    //std::vector<IChannel*> m_channels;
    m_channels.push_back(&m_VR);
    m_channels.push_back(&m_Vehicle);
    m_channels.push_back(&m_UI);
    m_channels.push_back(&m_TTS);
    m_channels.push_back(&m_Navi);
    m_channels.push_back(&m_Buttons);
    m_channels.push_back(&m_Base);

    pthread_t  thread_connect;
    pthread_create(&thread_connect,NULL,SDLConnector::onSetupConnection,this);
    return m_sdl_is_connected;
}

void SDLConnector::setUpConnecteion()
{
    m_sdl_is_connected = m_Sockets.ConnectTo(m_channels, this);
    if(m_sdl_is_connected){
    m_VR.onOpen();
    m_Vehicle.onOpen();
    m_UI.onOpen();
    m_TTS.onOpen();
    m_Navi.onOpen();
    m_Buttons.onOpen();

#ifdef WIN32
        Sleep(100);
#else
        usleep(100000);
#endif

        m_Base.onOpen();
    }
}

void* SDLConnector::onSetupConnection(void* arg)
{
    SDLConnector *cly=(SDLConnector*)arg;
    if(cly==NULL){
        LOGE("onSetupConnection is null");
        return NULL;
    }

    while(true){
        if(!cly->IsSDLConnected()){
             cly->setUpConnecteion();
        }
#ifdef WIN32
        Sleep(2000);
#else
        sleep(2);
#endif
    }
    return NULL;
}

void SDLConnector::ChangeMsgHandler(IMessageInterface * pMsgHandler)
{
    m_VR.SetCallback(pMsgHandler);
    m_Vehicle.SetCallback(pMsgHandler);
    m_UI.SetCallback(pMsgHandler);
    m_TTS.SetCallback(pMsgHandler);
    m_Navi.SetCallback(pMsgHandler);
    m_Buttons.SetCallback(pMsgHandler);
    m_Base.SetCallback(pMsgHandler);
}

void SDLConnector::OnAppActivated(int appID)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "BasicCommunication.OnAppActivated";
    params["appID"] = appID;
    root["params"] = params;

    m_Base.SendJson(root);
}

void SDLConnector::OnSoftButtonClick(int id, int mode)
{
    _onButtonClickAction("CUSTOM_BUTTON", "BUTTONDOWN", id);
    _onButtonClickAction("CUSTOM_BUTTON", "BUTTONUP", id);
    if(mode == BUTTON_SHORT)
        _onButtonClickAction("CUSTOM_BUTTON", "SHORT", id);
    else
        _onButtonClickAction("CUSTOM_BUTTON", "LONG", id);
}

void SDLConnector::_onButtonClickAction(std::string name, std::string mode, int customButtonID)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "Buttons.OnButtonEvent";
    params["name"] = name;
    params["mode"] = mode;
    params["customButtonID"] = customButtonID;
    root["params"] = params;

    m_Buttons.SendJson(root);
}

void SDLConnector::OnApplicationExit(int appID)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "BasicCommunication.OnExitApplication";
    params["appID"] = appID;
    root["params"] = params;

    m_Base.SendJson(root);
}

void SDLConnector::OnApplicationOut(int appID)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "BasicCommunication.OnAppDeactivated";
    params["appID"] = appID;
    params["reason"] = "GENERAL";
    root["params"] = params;

    m_Base.SendJson(root);
}

// reason 0:timeout 1:aborted 2:clickSB
void SDLConnector::OnAlertResponse(int id, int reason)
{
    Json::Value root;
    if(reason == ALERT_CLICK_SOFTBUTTON || reason == ALERT_TIMEOUT){
        Json::Value result;

        root["jsonrpc"] = "2.0";
        root["id"] = id;
        result["code"] = 0;
        result["method"] = "UI.Alert";
        root["result"] = result;
    }
    else{
        Json::Value error;
        Json::Value data;

        root["jsonrpc"] = "2.0";
        root["id"] = id;
        data["method"] = "UI.Alert";
        error["message"] = "Alert request aborted";
        error["code"] = 4;
        error["date"] = data;
        root["error"] = error;
    }
    m_UI.SendJson(root);
    m_UI.onSystemContext("MAIN");
}

void SDLConnector::OnScrollMessageResponse(int id, int reason)
{
    Json::Value root;
    if(reason == SCROLLMESSAGE_TIMEOUT || reason == SCROLLMESSAGE_CLICK_SOFTBUTTON){
        Json::Value result;

        root["jsonrpc"] = "2.0";
        root["id"] = id;
        result["code"] = 0;
        result["method"] = "UI.ScrollMessage";
        root["result"] = result;
    }
    else{
        Json::Value error;
        Json::Value data;

        root["jsonrpc"] = "2.0";
        root["id"] = id;
        data["method"] = "UI.ScrollMessage";
        error["message"] = "REJECTED";
        error["code"] = 4;
        error["date"] = data;
        root["error"] = error;
    }
    m_UI.SendJson(root);
    m_UI.onSystemContext("MAIN");
}

void SDLConnector::OnCommandClick(int appID, int cmdID)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "UI.OnCommand";
    params["appID"] = appID;
    params["cmdID"] = cmdID;
    root["params"] = params;

    m_UI.SendJson(root);
}

void SDLConnector::OnPerformInteraction(int code, int performInteractionID, int choiceID)
{
    Json::Value root;
    Json::Value result;

    root["jsonrpc"] = "2.0";
    root["id"] = performInteractionID;
    result["code"] = code;
    result["method"] = "UI.PerformInteraction";
    if(code == 0){
        result["choiceID"] = choiceID;
    }
    root["result"] = result;
    m_UI.SendJson(root);
}

void SDLConnector::OnVRCommand(int appID, int cmdID)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "VR.OnCommand";
    params["appID"] = appID;
    params["cmdID"] = cmdID;
    root["params"] = params;

    m_VR.SendJson(root);
}

void SDLConnector::OnSliderResponse(int code, int sliderid, int sliderPosition)
{
    Json::Value root;
    std::string info_msg = "";
    if (code == SLIDER_OK){
        Json::Value result;
        result["code"] = code;
        result["method"] = "UI.Slider";
        result["sliderPosition"] = sliderPosition;
        root["result"] = result;
        root["id"] = sliderid;
        root["jsonrpc"] = "2.0";
    }else{

        if(code == SLIDER_TIMEOUT){
            info_msg = "Slider request timeout.";
        }else{
            info_msg = "Slider request aborted.";
        }
        Json::Value error;
        Json::Value data;
        data["method"] = "UI.Slider";
        error["code"] = code;
        error["message"] = info_msg;
        error["data"] = data;
        root["id"] = sliderid;
        root["jsonrpc"] = "2.0";
        root["error"] = error;
    }

    m_UI.SendJson(root);
}

void SDLConnector::OnPerformAudioPassThru(int appID, int performaududiopassthruID, int code)
{
    _stopPerformAudioPassThru(appID);
    Json::Value root;
    if(code == 0){
        Json::Value result;
        result["code"] = code;
        result["method"] = "UI.PerformAudioPassThru";
        root["jsonrpc"] = "2.0";
        root["id"] = performaududiopassthruID;
        root["result"] = result;
        m_UI.SendJson(root);
    }else if(code == 5){
        Json::Value error;
        Json::Value data;

        data["method"] = "UI.PerformAudioPassThru";
        error["code"] = code;
        error["message"] = "PerformAudioPassThru was not completed successful!";
        error["data"] = data;
        root["jsonrpc"] = "2.0";
        root["id"] = performaududiopassthruID;
        root["error"] = error;
        m_UI.SendJson(root);
    }
    else{
        Json::Value error;
        Json::Value data;

        data["method"] = "UI.PerformAudioPassThru";
        error["code"] = code;
        error["message"] = "PerformAudioPassThru was not completed successful!";
        error["data"] = data;
        root["jsonrpc"] = "2.0";
        root["id"] = performaududiopassthruID;
        root["error"] = error;
        m_UI.SendJson(root);
    }
}

void SDLConnector::_stopPerformAudioPassThru(int appID)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "UI.PerformAudioPassThruStop";
    params["appID"] = appID;
    root["params"] = params;

    m_UI.SendJson(root);
}

void SDLConnector::OnButtonClick(std::string buttonname, int mode)
{
    _buttonEventDown(buttonname);
    _buttonPressed(buttonname, mode);
    _buttonEventUp(buttonname);
}

void SDLConnector::_buttonEventDown(std::string buttonname)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "Buttons.OnButtonEvent";
    params["mode"] = "BUTTONDOWN";
    params["name"] = buttonname;
    root["params"] = params;

    m_UI.SendJson(root);
}

void SDLConnector::_buttonPressed(std::string buttonname, int mode)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "Buttons.OnButtonEvent";
    if(mode == BUTTON_LONG)
        params["mode"] = "LONG";
    else
        params["mode"] = "SHORT";
    params["name"] = buttonname;
    root["params"] = params;

    m_UI.SendJson(root);
}

void SDLConnector::_buttonEventUp(std::string buttonname)
{
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["method"] = "Buttons.OnButtonEvent";
    params["mode"] = "BUTTONUP";
    params["name"] = buttonname;
    root["params"] = params;

    m_UI.SendJson(root);
}

void SDLConnector::OnTTSSpeek(int speekID, int code)
{
    if(code == SPEEK_OK){
        Json::Value root;
        Json::Value result;

        root["jsonrpc"] = "2.0";
        root["id"] = speekID;

        result["method"] = "TTS.Speek";
        result["code"] = code;

        root["result"] = result;

        m_TTS.SendJson(root);
    }else{
        Json::Value root;
        Json::Value error;
        Json::Value data;

        data["method"] = "TTS.Speek";

        root["jsonrpc"] = "2.0";
        root["id"] = speekID;

        error["message"] = "Speech was interrupted";
        error["code"] = code;
        error["data"] = data;

        root["result"] = error;

        m_TTS.SendJson(root);
    }
}

void SDLConnector::OnVRStartRecord()
{
    Json::Value root;

    root["jsonrpc"] = "2.0";
    root["method"] = "VR.StartRecord";

    m_VR.SendJson(root);
}
void SDLConnector::OnVRCancelRecord()
{
    Json::Value root;

    root["jsonrpc"] = "2.0";
    root["method"] = "VR.CancelRecord";

    m_VR.SendJson(root);
}

void SDLConnector::OnVideoScreenTouch(TOUCH_TYPE touch,int x,int y)
 {
     Json::Value root;
     Json::Value params;
     Json::Value coord;
     Json::Value event;
     Json::Value ts;
     static int id=0;
//     [{"c":[{"x":103,"y":247}]

     switch(touch){
     case TOUCH_START:
         id++;
         params["type"] = "BEGIN";
         event[0]["id"]=id;
         break;
     case TOUCH_END:
         params["type"] = "END";
         event[0]["id"]=0;
         break;
     case TOUCH_MOVE:
         params["type"] = "MOVE";
         event[0]["id"]=id;
         break;
     }
     coord[0]["x"]=x;
     coord[0]["y"]=y;
     event[0]["c"]=coord;

#ifdef WIN32
     SYSTEMTIME  time;
     GetSystemTime(&time);
     int   t = (time.wHour*3600+time.wMinute*60+time.wSecond)%1000*1000+time.wMilliseconds;
#else
     timeval val;
     gettimeofday(&val,NULL);
     int t=(int)(val.tv_sec%1000*1000+val.tv_usec/1000);
#endif
     ts[0]=t;
     event[0]["ts"]=ts;
     params["event"] =event;
     root["jsonrpc"] = "2.0";
     root["method"] = "UI.OnTouchEvent";
     root["params"] = params;

     LOGI("%s",root.toStyledString().data());

   //  std::cout<<root.asString();
     m_UI.SendJson(root);

 }


//{
//    "jsonrpc":"2.0",
//    "method":"UI.OnTouchEvent",
//    "params":
//    {
//        "type":"END",
//        "event":[
//            {"c":[
//                {
//                    "x":441,
//                    "y":158
//                }
//                ],
//            "id":0,
//            "ts":[87214]
//            }
//            ]
//    }
//}


