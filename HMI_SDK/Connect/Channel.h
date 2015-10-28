#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "ISocketManager.h"
#include "IMessageInterface.h"
#include <vector>
#ifdef ANDROID
#include <unistd.h>
#endif

class JsonBuffer
{
public:
	JsonBuffer();

	bool getJsonFromBuffer(char * pData, int iLength, Json::Value& output);

private:
	std::string m_szBuffer;
};

class Channel : public IChannel
{
public:
	Channel(std::string Channelname);
	virtual ~Channel();
    void SetCallback(IMessageInterface * pCallback);

	std::string	getChannelName();
	void setSocketManager(ISocketManager * pManager, void * pHandle);
	void onOpen();
	virtual void onReceiveData(void * pData, int iLength);

protected:
	void onMessage(Json::Value jsonObj);
	void unRegisterComponent();
	void sendError(int resultCode, int id, std::string method, std::string message);

	void SubscribeToNotification(std::string notification);
	void UnsubscribeFromNotification(std::string notification);


public:
	//IMessageCallback
	virtual void onRequest(Json::Value);
	virtual void onNotification(Json::Value);
	virtual void onResult(Json::Value);
	virtual void onRawData(void * p, int iLength);
	virtual void onError(std::string error);
    void SendJson(Json::Value data);

protected:
	virtual void onRegistered();
	virtual void onUnregistered();

	Json::Value getJsonFromNetworkData(void * p, int iLength);

private:
	void GenerateId();
	void ReadConfigure();

protected:
	int m_iIDStart;
	int m_iRequestId;
	int m_iRegisterRequestId;
	int m_iUnregisterRequestId;
	int m_iIDRange;
	std::string m_sComponentName;
	int m_iAppID;

	JsonBuffer m_JsonBuffer;

	ISocketManager * m_pSocketManager;
	void * m_pHandle;
    IMessageInterface * m_pCallback;

	Json::Value m_StaticConfigJson;
	Json::Value m_ResultCodeJson;
};

#endif
