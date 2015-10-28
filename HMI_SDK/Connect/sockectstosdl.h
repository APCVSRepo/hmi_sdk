#ifndef SOCKECTSTOSDL
#define SOCKECTSTOSDL
#include "ISocketManager.h"
#include <vector>
#include <queue>
#ifdef ANDROID
#include <pthread.h>
#else
#include "pthread/pthread.h"
#endif
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

typedef struct _SOCK_HANDLE
{
    IChannel * pDataReceiver;
    int socket;
    std::queue<SEND_DATA> m_SendData;
}SOCK_HANDLE;

class SocketsToSDL : public ISocketManager
{
public:
    SocketsToSDL();
    virtual ~SocketsToSDL();

public:
    bool ConnectTo(std::vector<IChannel *> Channels, INetworkStatus * pNetwork = 0);
    void SendData(void * pHandle, void * pData, int iLength);

    void RunThread();


private:
    bool CreateSignal();
    void Notify();
    bool Send(SOCK_HANDLE * pHandle);
    bool Receive(SOCK_HANDLE * pHandle);
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

    std::vector<SOCK_HANDLE *> m_SocketHandles;
};

#endif // SOCKECTSTOSDL

