#include "MediaCodecStream.h"

#define MAX_MSG 1024
#define __D(fmt,...)  logOut(__FILE__,__FUNCTION__,__LINE__,fmt"",##__VA_ARGS__)

extern uchar *gBuffer;
extern QMutex g_notifyMutex;
extern bool g_b_notify;
extern int g_msgNo, g_x, g_y;

static int DONE = 1;
static QQueue<FrameS> g_queueFrame;
static QQueue<DataS> g_queueData;
bool g_b_canFlush;
#ifdef PLAY_FILE_TEST
uchar readBuf[1024] = {0};
#endif
QMutex g_mutex;
AppListInterface *g_pList;
void *flushDataThread(void *args)
{
#if defined(ANDROID)
    while(DONE)
    {
        g_mutex.lock();
#ifdef FLUSH_FRAME
        if(!g_queueFrame.empty() && g_b_canFlush)
        {
            FrameS dataPackage = g_queueFrame.dequeue();
#else
        if(!g_queueData.empty() && g_b_canFlush)
        {
            DataS dataPackage = g_queueData.dequeue();
#endif
            if(gBuffer != NULL)
            {
//                QDBG << dataPackage.len;
                memcpy(gBuffer, dataPackage.buf, dataPackage.len);
                QAndroidJniObject::callStaticMethod<void>(
                            "an/qt/useJar/ExtendsQtSurface",
                            "flsh",
                            "(I)V",
                            dataPackage.len);
            }
        }
        g_mutex.unlock();
#ifdef FLUSH_FRAME
        usleep(20000);
#else
        usleep(2000);
#endif
    }
#endif
}
enum{
    MSG_NO_CAN_FLUSH = 101,
    MSG_NO_ZOOM_IN = 102,
    MSG_NO_ZOOM_OUT = 103,
    MSG_NO_MENU = 104,
    MSG_NO_PRESS = 105,
    MSG_NO_RELEASE = 106,
    MSG_NO_MOVE = 107,
    MSG_NO_CMD_1 = 111,
};
#define ZOOMINBTNID 3
#define ZOOMOUTBTNID 4
void *msgThread(void *args)
{
    while(DONE)
    {
        if(g_b_notify)
        {
            g_notifyMutex.lock();
            int x = g_x * 800.0/1024;
            int y = g_y * 480.0/600;
            g_b_notify = false;
            switch(g_msgNo)
            {
            case MSG_NO_CAN_FLUSH:
                g_b_canFlush = true;
                break;
            case MSG_NO_ZOOM_IN:
                if(g_pList != NULL)
                    if(g_pList->getActiveApp() != NULL)
                        g_pList->getActiveApp()->OnSoftButtonClick(ZOOMINBTNID,BUTTON_SHORT);
                break;
            case MSG_NO_ZOOM_OUT:
                if(g_pList != NULL)
                    if(g_pList->getActiveApp() != NULL)
                        g_pList->getActiveApp()->OnSoftButtonClick(ZOOMOUTBTNID,BUTTON_SHORT);
                break;
            case MSG_NO_MENU:
                if(g_pList != NULL)
                    if(g_pList->getActiveApp() != NULL)
                    {
                        QDBG << g_pList->getActiveApp()->getAppName().c_str();
                        //"¸ßµÂµ¼º½"
                        const char appName[13] = {0xE9, 0xAB, 0x98,
                                                  0xE5, 0xBE, 0xB7,
                                                  0xE5, 0xAF, 0xBC,
                                                  0xE8, 0x88, 0xAA, 0x00};
                        const char *name = g_pList->getActiveApp()->getAppName().c_str();
                        if(0 == strcmp(name, appName))
                        {
                            QDBG << "~~~~~~~~~~~~~~~~~~~~~~~!!!!!!";
                        }
                        else
                        {
                            QDBG << "~~~~~~~~~~~~123~~~~~~~~~~~!!!!!!";
                            QAndroidJniObject::callStaticMethod<void>(
                                        "an/qt/useJar/ExtendsQtSurface",
                                        "stop","()V");
                            g_pList->getActiveApp()->OnShowCommand();
                        }

                    }
                break;
            case MSG_NO_PRESS:
//                QDBG << g_x << g_y;
                if(g_pList != NULL)
                    if(g_pList->getActiveApp() != NULL)
                        g_pList->getActiveApp()->OnVideoScreenTouch(TOUCH_START,x,y);
                break;
            case MSG_NO_RELEASE:
//                QDBG << g_x << g_y;
                if(g_pList != NULL)
                    if(g_pList->getActiveApp() != NULL)
                        g_pList->getActiveApp()->OnVideoScreenTouch(TOUCH_END,x,y);
                break;
            case MSG_NO_MOVE:
//                QDBG << g_x << g_y;
                if(g_pList != NULL)
                    if(g_pList->getActiveApp() != NULL)
                        g_pList->getActiveApp()->OnVideoScreenTouch(TOUCH_MOVE,x,y);
                break;
            case MSG_NO_CMD_1:
                g_pList->OnAppExit();
                QAndroidJniObject::callStaticMethod<void>(
                            "an/qt/useJar/ExtendsQtSurface",
                            "stop","()V");
                break;
            }

            g_notifyMutex.unlock();
        }
        usleep(10000);
    }
}

MediaCodecStream::MediaCodecStream(AppListInterface * pList, QWidget *parent) : QWidget(parent)
{
    g_pList = pList;

    setWindowFlags(Qt::FramelessWindowHint);
    if(parent)
    {
        setGeometry(0,0,parent->width(),parent->height());
    }

    g_b_canFlush = false;
    pthread_create(&threadid,NULL,flushDataThread,NULL);

#ifdef PLAY_FILE_TEST
    fp = fopen("/mnt/sdcard/gaode.mp4", "rb");
#endif

    pthread_create(&msgThreadid,NULL,msgThread,NULL);

}

MediaCodecStream::~MediaCodecStream()
{
    QDBG;
    pthread_join(threadid,NULL);
    pthread_join(msgThreadid,NULL);
#ifdef PLAY_FILE_TEST
    fclose(fp);
#endif
}

void MediaCodecStream::showActivity()
{
    QDBG;
    m_b_canPushQueue = true;
    QAndroidJniObject action = QAndroidJniObject::fromString("intent_Extends_Qt_Surface");
    QAndroidJniObject intent("android/content/Intent",
                             "(Ljava/lang/String;)V",
                             action.object<jstring>());
    startActivity(intent, 0);
}

void MediaCodecStream::hideActivity()
{
    stopStream();
}

static int findFirstFrame = 0;
static int byteCount = 4;
static uchar frameData[576000] = {0};
static int lastIndex = 0;
void MediaCodecStream::startStream()
{
#ifdef ANDROID
#ifdef PLAY_FILE_TEST
    fseek(fp,0L,SEEK_SET);
#endif
    QDBG;
//    QAndroidJniObject::callStaticMethod<void>(
//                "an/qt/useJar/ExtendsQtSurface",
//                "initDecoder","()V");

    findFirstFrame = 0;
    byteCount = 4;
    lastIndex = 0;

    g_mutex.lock();
    g_queueFrame.clear();
    g_mutex.unlock();
#endif

}

void MediaCodecStream::stopStream()
{
#if defined(ANDROID)
    QDBG;
    QAndroidJniObject::callStaticMethod<void>(
                "an/qt/useJar/ExtendsQtSurface",
                "stop","()V");

    g_b_canFlush = false;
    m_b_canPushQueue = false;

    findFirstFrame = 0;
    byteCount = 4;
    lastIndex = 0;

    g_mutex.lock();
    g_queueFrame.clear();
    g_mutex.unlock();

#endif
}

Result MediaCodecStream::onRequest(Json::Value jsonObj){return RESULT_SUCCESS;}
void MediaCodecStream::onNotification(Json::Value jsonObj){}
void MediaCodecStream::onResult(Json::Value jsonObj){}
void MediaCodecStream::onError(std::string){}
void MediaCodecStream::onRawData(void* data, int len)
{
#ifdef PLAY_FILE_TEST
    fread(readBuf,len,1,fp);
    data = readBuf;
#endif
    if(!m_b_canPushQueue)
    {
        return;
    }
#ifdef FLUSH_FRAME


    frameData[0] = 0;
    frameData[1] = 0;
    frameData[2] = 0;
    frameData[3] = 1;
    int nPos = 0;
    int index = 0;
    int flag = 0;
    uchar *buffer = (uchar*)data;

    int canOnFrame = 0;
    while (nPos < len)
    {
        canOnFrame = 0;
        while (nPos < len)
        {
            flag = buffer[nPos++];
            if(findFirstFrame == 1)
            {
                frameData[byteCount++] = (uchar)flag;
            }
            if (flag == 0)
            {
                index = lastIndex + 1;
                while (flag == 0)
                {
                    if (nPos < len)
                    {
                        lastIndex = 0;
                        flag = buffer[nPos++];
                        if(findFirstFrame == 1)
                        {
                            frameData[byteCount++] = (uchar)flag;
                        }
                        index++;
                    }
                    else
                    {
                        lastIndex = index;
                        break;
                    }
                }
                if (flag == 1 && index >= 4)
                {
                    if(findFirstFrame == 0)
                        findFirstFrame = 1;
                    else
                    {
                        byteCount = byteCount - 4;
                        canOnFrame = 1;
                    }

                    break;
                }
            }
        }
        if(canOnFrame == 1)
        {
            FrameS newFrame;
            memcpy(newFrame.buf, frameData, byteCount);
            newFrame.len = byteCount;
            g_mutex.lock();
            g_queueFrame.enqueue(newFrame);

//            QDBG<<g_queueFrame.size()<<"byteCount:"<<byteCount;
            g_mutex.unlock();

            byteCount = 4;
        }
    }

#else
    DataS newData;
    memcpy(newData.buf, data, len);
    newData.len = len;
    g_mutex.lock();
    g_queueData.enqueue(newData);
    g_mutex.unlock();

#endif
}

void MediaCodecStream::OnClickedZoomInBtn()
{
//    ToSDL->OnSoftButtonClick(ZOOMINBTNID,BUTTON_SHORT);
}

void MediaCodecStream::OnClickedZoomOutBtn()
{
//    ToSDL->OnSoftButtonClick(ZOOMOUTBTNID,BUTTON_SHORT);
}

void MediaCodecStream::OnClickedMenuBtn()
{
//    m_pList->getActiveApp()->OnShowCommand();
}

