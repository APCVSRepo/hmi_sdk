#include "global_first.h"
#include <Connect/SocketsToSDL.h>

#ifdef WIN32
#ifdef WINCE
#pragma comment(lib,"ws2.lib")
#else
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"pthreadVC2.lib") 
#pragma comment(lib,"pthreadVCE2.lib") 
#pragma comment(lib,"pthreadVSE2.lib") 
#endif
#else
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

SocketsToSDL::SocketsToSDL()
	:	m_Read_Sign(-1), 
		m_Write_Sign(-1),
		m_SendMutex(),
		m_SendThread(),
        m_bTerminate(false)
{
    m_sHost = "127.0.0.1";
    m_iPort = 12346;
	pthread_mutex_init(&m_SendMutex, 0);

#ifdef WIN32
	WSADATA wsa = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsa); 
#endif
}

SocketsToSDL::~SocketsToSDL()
{
    m_bTerminate = true;
	Notify();
	pthread_join(m_SendThread, 0);
	pthread_mutex_destroy(&m_SendMutex);

    CloseSockets();
}

void SocketsToSDL::CloseSockets()
{
    if (-1 != m_Read_Sign)
#if defined(WIN32)
        closesocket(m_Read_Sign);
#else
        close(m_Read_Sign);
#endif

    if (-1 != m_Write_Sign)
#ifdef WIN32
        closesocket(m_Write_Sign);
#else
        close(m_Write_Sign);
#endif

    m_Read_Sign = -1;
    m_Write_Sign = -1;
    m_bTerminate = true;

    int iNum = m_SocketHandles.size();
    for (int i = 0; i < iNum; i++)
    {
        SOCK_HANDLE * pHandle = m_SocketHandles[i];
#ifdef WIN32
        closesocket(pHandle->socket);
#else
        close(pHandle->socket);
#endif
        while (!pHandle->m_SendData.empty())
        {
            SEND_DATA data = pHandle->m_SendData.front();
            pHandle->m_SendData.pop();
            ::free(data.pData);
        }
        delete pHandle;
    }
    m_SocketHandles.clear();
}

void SocketsToSDL::Notify()
{
    if (-1 == m_Write_Sign)
		return;
	
	char c = 0;
	::send(m_Write_Sign, (const char *)&c, 1, 0);
}

bool SocketsToSDL::CreateSignal()
{
    int tcp1, tcp2;
    sockaddr_in name;
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
#ifdef WIN32
    int namelen = sizeof(name);
#else
    size_t namelen = sizeof(name);;
#endif

    tcp1 = tcp2 = -1;
	int tcp = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp == -1){
		goto clean;
	}
	if (bind(tcp, (sockaddr*)&name, namelen) == -1){
		goto clean;
	}
	if (::listen(tcp, 5) == -1){
		goto clean;
	}
#ifdef ANDROID
    if (getsockname(tcp, (sockaddr*)&name, (socklen_t *)&namelen) == -1){
#else
    if (getsockname(tcp, (sockaddr*)&name, &namelen) == -1){
#endif
        goto clean;
	}
    tcp1 = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp1 == -1){
		goto clean;
	}
    if (-1 == connect(tcp1, (sockaddr*)&name, namelen)){
		goto clean;
	}
#ifdef ANDROID
    tcp2 = accept(tcp, (sockaddr*)&name, (socklen_t *)&namelen);
#else
    tcp2 = accept(tcp, (sockaddr*)&name, &namelen);
#endif
	if (tcp2 == -1){
		goto clean;
	}
#ifdef WIN32
    if (closesocket(tcp) == -1){
#else
    if (close(tcp) == -1){
#endif
        goto clean;
	}
	m_Write_Sign = tcp1;
	m_Read_Sign = tcp2;
#ifdef WIN32
    {
        u_long iMode = 1;
        ioctlsocket(m_Read_Sign, FIONBIO, (u_long FAR*) &iMode);
    }
#else
    #include<fcntl.h>
    fcntl(m_Read_Sign,F_SETFL, O_NONBLOCK);
#endif
    return true;

clean:
	if (tcp != -1){
#ifdef WIN32
        closesocket(tcp);
#else
        close(tcp);
#endif
    }
	if (tcp2 != -1){
#ifdef WIN32
        closesocket(tcp2);
#else
        close(tcp2);
#endif
    }
	if (tcp1 != -1){
#ifdef WIN32
        closesocket(tcp1);
#else
        close(tcp1);
#endif
    }
	return false;
}

void* StartSocketThread(void* p) {
	SocketsToSDL * pThis = (SocketsToSDL *)p;
	pThis->RunThread();
	return 0;
}

bool SocketsToSDL::ConnectTo(std::vector<IChannel *> Channels, INetworkStatus * pNetwork)
{
    m_pNetwork = pNetwork;
	if (!CreateSignal())
		return false;

    int iNum = Channels.size();
    for (int i = 0; i < iNum; i++)
	{
        SOCK_HANDLE * pHandle = getNewSocketHandle(Channels[i], m_sHost, m_iPort);

        if(pHandle != NULL)
        {
            m_SocketHandles.push_back(pHandle);
            Channels[i]->setSocketManager(this, pHandle);
        }
        else
        {
            goto FAILED;
        }
	}

    m_bTerminate = false;
	if (0 != pthread_create(&m_SendThread, 0, &StartSocketThread, this))
		goto FAILED;

    return true;

FAILED:
    CloseSockets();
    return false;
}

SOCK_HANDLE * SocketsToSDL::getNewSocketHandle(IChannel * newChannel, std::string sIP, int iPort)
{
    sockaddr_in toLocal;
    memset(&toLocal, 0, sizeof(toLocal));
    toLocal.sin_family = AF_INET;
    toLocal.sin_addr.s_addr = inet_addr(sIP.c_str());
    toLocal.sin_port = htons(iPort);

#ifdef WIN32
    int namelen = sizeof(toLocal);
#else
    size_t namelen = sizeof(toLocal);
#endif

    SOCK_HANDLE * pHandle = new SOCK_HANDLE();
    pHandle->pDataReceiver = newChannel;
    pHandle->socket = socket(AF_INET, SOCK_STREAM, 0);
    if(pHandle->socket==SOCKET_ERROR){
        LOGE("SOCKET INVALID");
    }
    try
    {
        if (SOCKET_ERROR == ::connect(pHandle->socket, (const sockaddr *)&toLocal, namelen))
            goto FAILED;
#ifdef WIN32
        {
            u_long iMode = 1;
            if (SOCKET_ERROR == ioctlsocket(pHandle->socket, FIONBIO, (u_long FAR*) &iMode))
                goto FAILED;
        }
#else
        fcntl(pHandle->socket, F_SETFL, O_NONBLOCK);
#endif
    }
    catch (...)
    {
        goto FAILED;
    }
    return pHandle;
FAILED:
#if defined(WIN32)
    closesocket(pHandle->socket);
#else
    close(pHandle->socket);
#endif
    delete pHandle;

    return NULL;
}

bool SocketsToSDL::ConnectToVS( IChannel * ChannelVS, std::string sIP, int iPort,INetworkStatus * pNetwork)
{

    m_pNetwork = pNetwork;

    SOCK_HANDLE * pHandle = getNewSocketHandle(ChannelVS, sIP, iPort);

    if(pHandle != NULL)
    {
        m_SocketHandles.push_back(pHandle);
        ChannelVS->setSocketManager(this, pHandle);
        Notify();
        return true;
    }
    else
    {
        return false;
    }
}
void SocketsToSDL::DelConnectToVS()
{
    SOCK_HANDLE * pHandle = m_SocketHandles.at(m_SocketHandles.size() - 1);

     m_SocketHandles.pop_back();
     usleep(1000000);
     shutdown(pHandle->socket, SHUT_RDWR);
//#if defined(WIN32)
//    closesocket(pHandle->socket);
//#else
//    close(pHandle->socket);
//#endif

}

void SocketsToSDL::SendData(void * pHandle, void * pData, int iLength)
{
    if(m_bTerminate)
        return;

    SOCK_HANDLE * p = (SOCK_HANDLE *)pHandle;
	SEND_DATA data;
	data.iLength = iLength;

    void * pBuffer = ::malloc(iLength);
	memcpy(pBuffer, pData, iLength);
	data.pData = pBuffer;

	pthread_mutex_lock(&m_SendMutex);
	p->m_SendData.push(data);
	pthread_mutex_unlock(&m_SendMutex);
	Notify();
}

bool SocketsToSDL::Send(SOCK_HANDLE * pHandle)
{
    bool bRet = true;
	pthread_mutex_lock(&m_SendMutex);
	while (!pHandle->m_SendData.empty())
	{
		SEND_DATA data = pHandle->m_SendData.front();
		try
		{
			int total = 0;
			do
			{
				int iSent = ::send(pHandle->socket, (const char *)data.pData + total, data.iLength - total, 0);
				if (SOCKET_ERROR == iSent)
				{
                    bRet = false;
					break;
				}
				total += iSent;
			} while (total < data.iLength);
			if (total < data.iLength)
				break;
			pHandle->m_SendData.pop();
            ::free(data.pData);
		}
		catch (...)
		{
            bRet = false;
            break;
        }
	}
	pthread_mutex_unlock(&m_SendMutex);
    return bRet;
}

bool SocketsToSDL::Receive(SOCK_HANDLE * pHandle)
{
	unsigned char buffer[1024];
	int bytes_read = 0;

    bool bRet = false;
	do
	{
		try
		{
			bytes_read = recv(pHandle->socket, (char *)buffer, sizeof(buffer), 0);
//            LOGE("pHandle->socket = %d,   bytes_read = %d", pHandle->socket, bytes_read);
        }
        catch (...)
		{
			return false;
		}
		if (bytes_read > 0)
			pHandle->pDataReceiver->onReceiveData(buffer, bytes_read);
        else if (SOCKET_ERROR == bytes_read)
           break;
        bRet = true;
    } while (bytes_read > 0);

    return bRet;
}

void SocketsToSDL::RunThread()
{
#ifdef WIN32
    FD_SET fdRead;
    int fd_max = -1;
#else
    fd_set fdRead;
    fd_set fdWrite;
    int fd_max = m_Read_Sign;
#endif
	while (!m_bTerminate)
	{
		FD_ZERO(&fdRead);
#ifndef WIN32
		FD_ZERO(&fdWrite);
#endif

		int iNum = m_SocketHandles.size();
        for (int i = 0; i < iNum; i++)
		{
			SOCK_HANDLE * pHandle = m_SocketHandles[i];
			int socket = pHandle->socket;
			FD_SET(socket, &fdRead);
#ifndef WIN32
			FD_SET(socket, &fdWrite);
#endif
            fd_max = fd_max > socket? fd_max : socket;
		}
		FD_SET(m_Read_Sign, &fdRead);

#ifdef WIN32
        if (select(0, &fdRead, NULL, NULL, NULL) == SOCKET_ERROR)
#else
        if (select(fd_max+1, &fdRead, &fdWrite, NULL, NULL) == SOCKET_ERROR)
#endif
		{
            goto SOCKET_WRONG;
		}

        bool bSend = FD_ISSET(m_Read_Sign, &fdRead);
		if (bSend)
		{
			unsigned char buffer[8];
			int bytes_read = 0;
			do
			{
				bytes_read = recv(m_Read_Sign, (char *)buffer, sizeof(buffer), 0);
			} while (bytes_read > 0);
            iNum = m_SocketHandles.size();
            for (int i = 0; i < iNum; i++)
            {
                if(i + 1 > m_SocketHandles.size())
                {
                    break;
                }
                SOCK_HANDLE * pHandle = m_SocketHandles[i];
                if(!Send(pHandle))
                    goto SOCKET_WRONG;
            }
		}


        iNum = m_SocketHandles.size();
        for (int i = 0; i < iNum; i++)
		{
            if(i + 1 > m_SocketHandles.size())
            {
                break;
            }
			SOCK_HANDLE * pHandle = m_SocketHandles[i];
			if (FD_ISSET(pHandle->socket, &fdRead))
			{
                if(!Receive(pHandle))
                {
                    goto SOCKET_WRONG;
                }
            }
		}
	}
    return;

SOCKET_WRONG:
    m_bTerminate = true;
    CloseSockets();

    if(m_pNetwork)
        m_pNetwork->onNetworkBroken();
    return;
}
