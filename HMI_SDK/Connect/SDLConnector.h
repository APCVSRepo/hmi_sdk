#ifndef __SDL_CONNECTOR_H__
#define __SDL_CONNECTOR_H__
#include "Connect/ISocketManager.h"

#include "Connect/basecommunicationclient.h"
#include "Connect/buttonsclient.h"
#include "Connect/navigationclient.h"
#include "Connect/ttsclient.h"
#include "Connect/uiclient.h"
#include "Connect/vehicleinfoclient.h"
#include "Connect/vrclient.h"
#include "Connect/socketstosdl.h"
#include <string>
#include "json/json.h"
#include <stdio.h>
#ifdef ANDROID
#include <unistd.h>
#endif

#define ALERT_TIMEOUT                   0
#define ALERT_CLICK_SOFTBUTTON          1
#define ALERT_ABORTED                   2

#define SCROLLMESSAGE_TIMEOUT           0
#define SCROLLMESSAGE_CLICK_SOFTBUTTON  1
#define SCROLLMESSAGE_REJECTED          2

#define BUTTON_SHORT                    0
#define BUTTON_LONG                     1

#define PERFORMINTERACTION_TIMEOUT      10
#define PERFORMINTERACTION_CHOICE       0

#define SLIDER_OK                       0
#define SLIDER_TIMEOUT                  10
#define SLIDER_ABORTED                  5

#define PERFORMAUDIOPASSTHRU_TIMEOUT    0
#define PERFORMAUDIOPASSTHRU_RETYP      7
#define PERFORMAUDIOPASSTHRU_DONE       0
#define PERFORMAUDIOPASSTHRU_CANCEL     5

#define SPEEK_OK                        0
#define SPEEK_INTERRUPTED               5

typedef enum
{
    TOUCH_START,
    TOUCH_MOVE,
    TOUCH_END
}TOUCH_TYPE;

#define ToSDL SDLConnector::getSDLConnector()

class SDLConnector : public INetworkStatus
{
private:
    SDLConnector();
    ~SDLConnector();

public:
    static SDLConnector * getSDLConnector();
    static void Close();

    bool ConnectToSDL(IMessageInterface * pMsgHandler, INetworkStatus * pNetwork = NULL);
    void ChangeMsgHandler(IMessageInterface * pMsgHandler);
    bool IsSDLConnected();

private:
    bool m_bReleased;
    SocketsToSDL m_Sockets;
    std::vector<IChannel*> m_channels;
    bool    m_sdl_is_connected;

    vrClient m_VR;
    baseCommunicationClient m_Base;
    buttonsClient m_Buttons;
    navigationClient m_Navi;    
    ttsClient m_TTS;
    vehicleInfoClient m_Vehicle;
    uiClient m_UI;

    INetworkStatus * m_pNetwork;
    IMessageInterface * m_pMsgHandler;

public:
    virtual void onConnected();
    virtual void onNetworkBroken();
    static void* ConnectThread(void*);
    void Connect();

// API
public:
    void OnAppActivated(int appID);
    void OnAppExit(int appID);
    void OnAppOut(int appID);

    // mode: SHORT or LONG
    void OnSoftButtonClick(int sbID, int mode);

    // mode: SHORT or LONG
    void OnButtonClick(std::string buttonname, int mode);

    // reason: timeout clickSB and aborted
    void OnAlertResponse(int alertID, int reason);

    // reason: timeout clickSB and rejected
    void OnScrollMessageResponse(int smID, int reason);

    void OnCommandClick(int appID,int cmdID);
    void OnVRCommand(int appID,int cmdID);

    // code: timeout or choice
    void OnPerformInteraction(int code, int performInteractionID, int choiceID);

    // code: timeout sborted select someone
    void OnSliderResponse( int code, int sliderid, int sliderPosition);

    // code: timeout retry cancel
    void OnPerformAudioPassThru(int appID, int performaudiopassthruID, int code);

    // code: OK or Interrupted
    void OnTTSSpeek(int speekID, int code);

    void OnVRStartRecord(void);
    void OnVRCancelRecord(void);

    //touch event
    void OnVideoScreenTouch(TOUCH_TYPE touch,int x,int y);

private:
    void _onButtonClickAction(std::string, std::string, int);
    void _stopPerformAudioPassThru(int);
    void _buttonEventDown(std::string buttonname);
    void _buttonPressed(std::string buttonname, int mode);
    void _buttonEventUp(std::string buttonname);
};

#endif
