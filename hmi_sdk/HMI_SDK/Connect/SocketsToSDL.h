#ifndef SOCKETSTOSDL
#define SOCKETSTOSDL
#include "ISocketManager.h"
#include <vector>
#include <queue>
#include <pthread.h>
#include "IMessageInterface.h"

#ifdef WIN32
#include <WINSOCK2.H> 
#include <stdio.h> 
#endif

typedef struct _SEND_DATA
{
    void * pData;
    int iLength;
}SEND_DATA;

class CSockHandle
{
public:
    CSockHandle(int bufSize = 1024);
    ~CSockHandle();
    bool connect(IChannel * newChannel, std::string sIP, int iPort);
    void pushData(SEND_DATA data);
    bool sendData();
    bool recvData();
    void clear();

    IChannel * pDataReceiver;
    int m_i_socket;
    std::queue<SEND_DATA> m_SendData;
private:
    unsigned char *m_recBuf;
    int m_i_recBufSize;

    mutable pthread_mutex_t m_SendMutex;
};

class SocketsToSDL : public ISocketManager
{
public:
    SocketsToSDL();
    virtual ~SocketsToSDL();

public:
    bool ConnectTo(std::vector<IChannel *> Channels, INetworkStatus * pNetwork = 0);
    bool ConnectToVS(IChannel * ChannelVS, std::string sIP, int iPort, INetworkStatus * pNetwork = 0);
    void DelConnectToVS();
    void SendData(void * pHandle, void * pData, int iLength);

    void RunThread();

private:
    bool CreateSignal();
    void Notify();
    void CloseSockets();

private:
    int m_Read_Sign;
    int m_Write_Sign;

    mutable pthread_mutex_t m_SendMutex;
    pthread_t m_SendThread;
    bool m_bTerminate;

    INetworkStatus * m_pNetwork;

private:
    std::string m_sHost;
    int m_iPort;

    std::vector<CSockHandle *> m_SocketHandles;
};

#endif // SOCKECTSTOSDL

