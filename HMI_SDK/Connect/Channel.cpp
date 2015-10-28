//#include "HMISDK/include/global_first.h"
#ifdef WINCE
#else
#include <sys/stat.h>
#endif
#include <iostream>
#include <stdio.h>
#include <string>
#include <json/json.h>
#include <fstream>
#include <cassert>
#include<Connect/Channel.h>
#ifdef WIN32
#ifdef WINCE
#include "unistd.h"
#else
#include <direct.h>
#endif
#else
//#include <unistd.h>
#endif
#include<stdlib.h>

JsonBuffer::JsonBuffer()
{
    m_szBuffer = "";
}

bool JsonBuffer::getJsonFromBuffer(char * pData, int iLength, Json::Value& output)
{
    if (pData != 0 && iLength > 0)
        m_szBuffer.append(pData, iLength);
    if (m_szBuffer.length() < 10)
        return false;

    int pos = m_szBuffer.find_first_of('\n');
    if (int(std::string::npos) == pos)
        return false;

    std::string szFirstJson = m_szBuffer.substr(0, pos + 1);
    if ((pos + 1) >= m_szBuffer.length())
        m_szBuffer = "";
    else
        m_szBuffer = m_szBuffer.substr(pos + 1);

    Json::Reader reader;
    if (reader.parse(szFirstJson, output))
    {
        return true;

    }
    return false;
}


Channel::Channel(std::string Channelname)
{
    m_iIDStart = -1;
    m_iRequestId = -1;

    m_iAppID = -1;

    m_iRegisterRequestId = -1;
    m_iUnregisterRequestId = -1;
    m_iIDRange = 1000;

    m_sComponentName = Channelname;

    ReadConfigure();
}

Channel::~Channel()
{

}

void Channel::ReadConfigure()
{
    // init staticConfigDB
    std::ifstream ifs;
    char szPath[1024];
#ifdef WIN32
#ifdef WINCE
    wchar_t wszPath[1024];
    GetModuleFileName( NULL, wszPath, MAX_PATH );
    wchar_t *lpwszSplit = wcsrchr(wszPath, '\\');
    *lpwszSplit = 0;
    int nLength = WideCharToMultiByte(CP_ACP, 0, wszPath, -1, NULL, 0, NULL, NULL);
    memset(szPath, 0, sizeof(szPath));
    WideCharToMultiByte(CP_ACP, 0, wszPath, -1, szPath, nLength, NULL, NULL);
#else
    _getcwd(szPath,1024);
#endif
#else
    getcwd(szPath,1024);
#endif
    char szDB[1024];
#ifdef WIN32
    ::sprintf(szDB, "%s\\%s", szPath, "config\\staticConfigDB.json");//..\\hmi-sdk-plus\\HMISDK\\
#else
#ifdef ANDROID
//    ::sprintf(szDB, "%s", "/mnt/udisk/staticConfigDB.json");
#else
   // ::sprintf(szDB, "%s/%s", szPath, "HMISDK/config/staticConfigDB.json");
#endif
#endif


#ifdef ANDROID
    const char* str ="{\
    \"vehicleData\": {\
      \"bodyInformation\": {\
        \"parkBrakeActive\": false, \
        \"ignitionStableStatus\": \"MISSING_FROM_TRANSMITTER\", \
        \"ignitionStatus\": \"UNKNOWN\"\
      },\
      \"engineTorque\": 650, \
      \"vin\": \"52-452-52-752\", \
      \"batteryPackCurrent\": 7.0, \
      \"deviceStatus\": {\
        \"battLevelStatus\": \"NOT_PROVIDED\", \
        \"signalLevelStatus\": \"NOT_PROVIDED\", \
        \"primaryAudioSource\": \"NO_SOURCE_SELECTED\", \
        \"voiceRecOn\": false, \
        \"monoAudioOutputMuted\": false, \
        \"callActive\": false, \
        \"phoneRoaming\": false, \
        \"btIconOn\": false, \
        \"stereoAudioOutputMuted\": false, \
        \"textMsgAvailable\": false, \
        \"eCallEventActive\": false\
      }, \
      \"satRadioESN\": \"165165650\", \
      \"tripOdometer\": 0, \
      \"rpm\": 5000, \
      \"prndl\": \"PARK\", \
      \"fuelLevel_State\": \"UNKNOWN\", \
      \"batteryPackTemperature\": 30, \
      \"genericbinary\": \"165165650\", \
      \"wiperStatus\": \"NO_DATA_EXISTS\", \
      \"fuelLevel\": 0.2, \
      \"speed\": 80.0, \
      \"beltStatus\": {\
        \"passengerBuckleBelted\": \"NOT_SUPPORTED\", \
        \"middleRow1BeltDeployed\": \"NOT_SUPPORTED\", \
        \"middleRow1BuckleBelted\": \"NOT_SUPPORTED\", \
        \"passengerChildDetected\": \"NOT_SUPPORTED\", \
        \"rightRearInflatableBelted\": \"NOT_SUPPORTED\", \
        \"middleRow2BuckleBelted\": \"NOT_SUPPORTED\", \
        \"middleRow3BuckleBelted\": \"NOT_SUPPORTED\", \
        \"driverBeltDeployed\": \"NOT_SUPPORTED\", \
        \"leftRearInflatableBelted\": \"NOT_SUPPORTED\", \
        \"passengerBeltDeployed\": \"NOT_SUPPORTED\", \
        \"driverBuckleBelted\": \"NOT_SUPPORTED\", \
        \"rightRow3BuckleBelted\": \"NOT_SUPPORTED\", \
        \"leftRow2BuckleBelted\": \"NOT_SUPPORTED\", \
        \"leftRow3BuckleBelted\": \"NOT_SUPPORTED\", \
        \"rightRow2BuckleBelted\": \"NOT_SUPPORTED\"\
      }, \
      \"steeringWheelAngle\": 1.2, \
      \"driverBraking\": \"NOT_SUPPORTED\", \
      \"batteryPackVoltage\": 12.5, \
      \"gps\": {\
        \"compassDirection\": \"SOUTHWEST\", \
        \"actual\": false, \
        \"hdop\": 5, \
        \"utcMonth\": 2, \
        \"speed\": 2, \
        \"utcDay\": 14, \
        \"utcSeconds\": 54, \
        \"latitudeDegrees\": -830464, \
        \"pdop\": 15, \
        \"utcYear\": 2013, \
        \"utcHours\": 13, \
        \"vdop\": 30, \
        \"satellites\": 8, \
        \"altitude\": 7, \
        \"longitudeDegrees\": 423293, \
        \"utcMinutes\": 16, \
        \"heading\": 173, \
        \"dimension\": \"2D\"\
      }, \
      \"batteryVoltage\": 12.5, \
      \"externalTemperature\": 40.0, \
      \"instantFuelConsumption\": 2.2, \
      \"headLampStatus\": {\
        \"highBeamsOn\": false, \
        \"lowBeamsOn\": false\
      }, \
      \"tirePressure\": \"UNKNOWN\", \
      \"avgFuelEconomy\": 0.1, \
      \"myKey\": {\
        \"e911Override\": \"NO_DATA_EXISTS\"\
      }, \
      \"rainSensor\": 165165650, \
      \"accPedalPosition\": 0.5\
    }, \
    \"vehicleType\": {\
      \"trim\": \"SE\", \
      \"make\": \"Ford\", \
      \"model\": \"Fiesta\", \
      \"modelYear\": \"2015\"\
    }, \
    \"sdlLanguagesList\": [\
      \"EN-US\", \
      \"ES-MX\", \
      \"FR-CA\", \
      \"DE-DE\", \
      \"ES-ES\", \
      \"EN-GB\", \
      \"RU-RU\", \
      \"TR-TR\", \
      \"PL-PL\", \
      \"FR-FR\", \
      \"IT-IT\", \
      \"SV-SE\", \
      \"PT-PT\", \
      \"NL-NL\", \
      \"ZH-TW\", \
      \"JA-JP\", \
      \"AR-SA\", \
      \"KO-KR\", \
      \"PT-BR\", \
      \"CS-CZ\", \
      \"DA-DK\", \
      \"NO-NO\"\
    ], \
    \"capabilities\": [\
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"PRESET_0\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"PRESET_1\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"PRESET_2\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"PRESET_3\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"PRESET_4\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"PRESET_5\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"PRESET_6\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"PRESET_7\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"PRESET_8\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"PRESET_9\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"OK\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"SEEKLEFT\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"SEEKRIGHT\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"TUNEUP\", \
        \"shortPressAvailable\": true\
      }, \
      {\
        \"longPressAvailable\": true, \
        \"upDownAvailable\": true, \
        \"name\": \"TUNEDOWN\", \
        \"shortPressAvailable\": true\
      }\
    ], \
    \"hmiTTSVRLanguage\": \"EN-US\", \
    \"hmiUILanguage\": \"EN-US\", \
    \"hmiVRLanguage\": \"EN-US\", \
    \"capabilities_tts\":[\"TEXT\"],\
    \"resultCode\": {\
      \"INVALID_ID\": 13, \
      \"RETRY\": 7, \
      \"WRONG_LANGUAGE\": 16, \
      \"IGNORED\": 6, \
      \"IN_USE\": 8, \
      \"CHAR_LIMIT_EXCEEDED\": 12, \
      \"USER_DISALLOWED\": 23, \
      \"GENERIC_ERROR\": 22, \
      \"DISALLOWED\": 3, \
      \"APPLICATION_NOT_REGISTERED\": 15, \
      \"SUCCESS\": 0, \
      \"WARNINGS\": 21, \
      \"UNSUPPORTED_RESOURCE\": 2, \
      \"TOO_MANY_PENDING_REQUESTS\": 18, \
      \"REJECTED\": 4, \
      \"INVALID_DATA\": 11, \
      \"DUPLICATE_NAME\": 14, \
      \"DATA_NOT_AVAILABLE\": 9, \
      \"UNSUPPORTED_REQUEST\": 1, \
      \"TIMED_OUT\": 10, \
      \"OUT_OF_MEMORY\": 17, \
      \"ABORTED\": 5, \
      \"NO_APPS_REGISTERED\": 19, \
      \"NO_DEVICES_CONNECTED\": 20\
    },\
    \"uiGetCapabilitiesResult\": {\
      \"softButtonCapabilities\": [\
        {\
          \"longPressAvailable\": true, \
          \"upDownAvailable\": true, \
          \"imageSupported\": true, \
          \"shortPressAvailable\": true\
        }\
      ], \
      \"method\": \"UI.GetCapabilities\", \
      \"displayCapabilities\": {\
        \"textFields\": [\
          \"mainField1\", \
          \"mainField2\", \
          \"mainField1\", \
          \"mainField2\", \
          \"statusBar\", \
          \"mediaClock\", \
          \"mediaTrack\", \
          \"alertText1\", \
          \"alertText2\", \
          \"alertText3\", \
          \"scrollableMessageBody\", \
          \"initialInteractionText\", \
          \"navigationText1\", \
          \"navigationText2\", \
          \"ETA\", \
          \"totalDistance\", \
          \"navigationText\", \
          \"audioPassThruDisplayText1\", \
          \"audioPassThruDisplayText2\", \
          \"sliderHeader\", \
          \"sliderFooter\", \
          \"notificationText\"\
        ], \
        \"displayType\": \"GEN2_8_DMA\", \
        \"mediaClockFormats\": [\
          \"CLOCK1\", \
          \"CLOCK2\", \
          \"CLOCK3\", \
          \"CLOCKTEXT1\", \
          \"CLOCKTEXT2\", \
          \"CLOCKTEXT3\", \
          \"CLOCKTEXT4\"\
        ], \
        \"imageCapabilities\": [\
          \"DYNAMIC\"\
        ]\
      }, \
      \"hmiZoneCapabilities\": [\
        \"FRONT\"\
      ], \
      \"code\": 0\
    }\
  }";

     Json::Reader reader;
     if (reader.parse(str, m_StaticConfigJson))  // reader将Json字符串解析到root，root将包含Json里所有子元素
     {
        m_ResultCodeJson = m_StaticConfigJson["resultCode"];
     }
#else
    std::cout << szDB <<"\n";
    std::cout<<szPath<<"\n";
    ifs.open(szDB);
    assert(ifs.is_open());

    Json::Reader reader;

    if (!reader.parse(ifs, m_StaticConfigJson, false))
    {
        std::cout << "staticConfigJson error.\n";
    }
    else
    {
        m_ResultCodeJson = m_StaticConfigJson["resultCode"];
    }
    ifs.close();
#endif
}

void Channel::unRegisterComponent()
{
    m_iRegisterRequestId = m_iIDStart;
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iRegisterRequestId;
    root["method"] = "MB.unregisterComponent";

    params["componentName"] = m_sComponentName;

    root["params"] = params;
    SendJson(root);
}

void Channel::SetCallback(IMessageInterface * pCallback)
{
    m_pCallback = pCallback;
}

void Channel::setSocketManager(ISocketManager * pSocketManager, void * pHandle)
{
    m_pSocketManager = pSocketManager;
    m_pHandle = pHandle;
}

std::string	Channel::getChannelName()
{
    return m_sComponentName;
}

void Channel::onReceiveData(void * pData, int iLength)
{
    Json::Value RPC;
    while (m_JsonBuffer.getJsonFromBuffer((char*)pData, iLength, RPC))
    {
        onMessage(RPC);
        pData = 0;
        iLength = 0;
    }
}

void Channel::sendError(int resultCode, int id, std::string method, std::string message){
    Json::Value root;
    Json::Value error;
    Json::Value data;

    data["method"] = method;
    error["code"] = resultCode;
    error["data"] = data;
    error["message"] = message;

    root["jsonrpc"] = "2.0";
    root["id"] = id;
    root["error"] = error;

    SendJson(root);
}

void Channel::SendJson(Json::Value data)
{
    Json::FastWriter writer;
    std::string json_file = writer.write(data);
    const char * pStr = json_file.c_str();
    m_pSocketManager->SendData(m_pHandle, (void *)pStr, json_file.length());
}

void Channel::onMessage(Json::Value jsonObj)
{
    std::string _methon = "";
    if (jsonObj.isMember("method") && jsonObj["method"].asString() == "BasicCommunication.SDLLog")
    {
        _methon = jsonObj["method"].asString();
    }

    if (jsonObj.isMember("params"))
    {
        if (jsonObj["params"].isMember("app_id"))
        {
            m_iAppID = jsonObj["params"]["app_id"].asInt();
        }
    }
    // id
    if (jsonObj.isMember("id"))
    {
        if (jsonObj["id"].asInt() == m_iRegisterRequestId)
        {
            if (!jsonObj.isMember("error"))
            {
                if (jsonObj.isMember("result"))
                {
                    m_iRequestId = jsonObj["result"].asInt();
                    m_iIDStart = jsonObj["result"].asInt();
                    onRegistered();
                }
            }
        }
        // handle component unregistration
        else if (jsonObj["id"].asInt() == m_iUnregisterRequestId)
        {
            if (!jsonObj.isMember("error"))
            {
                onUnregistered();
            }
        }
        // handle result, error, notification, requests
        else
        {
            if (jsonObj.isMember("result"))
            {
                onResult(jsonObj);
            }
            else if (jsonObj.isMember("error"))
            {
                onError(jsonObj["error"].asString());
            }
            else
            {
                onRequest(jsonObj);
            }
        }
    }
    else
    {
        onNotification(jsonObj);
    }
}

void Channel::onOpen()
{
    m_iRegisterRequestId = m_iIDStart;

    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iIDStart;
    root["method"] = "MB.registerComponent";

    params["componentName"] = m_sComponentName;

    root["params"] = params;
    SendJson(root);
}

void Channel::onRegistered()
{
}

void Channel::onUnregistered()
{

}

void Channel::GenerateId()
{
    m_iRequestId = m_iRequestId + 1;
    if (m_iRequestId >= m_iIDStart + m_iIDRange)
    {
        m_iRequestId = m_iIDStart;
    }
}

void Channel::onRequest(Json::Value data)
{
    m_pCallback->onRequest(data);
}

void Channel::onNotification(Json::Value data)
{
    m_pCallback->onNotification(data);
}
void Channel::onResult(Json::Value data)
{
    m_pCallback->onResult(data);
}
void Channel::onRawData(void * p, int iLength)
{
    m_pCallback->onRawData(p, iLength);
}

void Channel::onError(std::string error)
{
    m_pCallback->onError(error);
}

void Channel::SubscribeToNotification(std::string notification)
{
    GenerateId();
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iRequestId;
    root["method"] = "MB.subscribeTo";

    params["propertyName"] = notification;

    root["params"] = params;
    SendJson(root);
}

void Channel::UnsubscribeFromNotification(std::string notification)
{
    GenerateId();
    Json::Value root;
    Json::Value params;

    root["jsonrpc"] = "2.0";
    root["id"] = m_iRequestId;
    root["method"] = "MB.unsubscribeTo";

    params["propertyName"] = notification;

    root["params"] = params;
    SendJson(root);
}
