#ifndef JNIFRAME_H
#define JNIFRAME_H

#include <QObject>
#if defined(ANDROID)
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <jni.h>
#include <unistd.h>
#include <android/log.h>
#endif
#include <QTimer>
#include <QQueue>
#include <QMutex>
#include "JniNative.h"
#include "AppListInterface.h"
#define QDBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__
#if defined(ANDROID)
#define __D(fmt,...)  logOut(__FILE__,__FUNCTION__,__LINE__,fmt"",##__VA_ARGS__)
#include <stdio.h>
#include <stdarg.h>

#define MAX_MSG     1000

static int logOut(const char *file, const char *func, const int line, const char *fmt, ...)
{
    char msg[MAX_MSG]={0};
    if(NULL != fmt)
    {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(msg, sizeof(msg), fmt, ap);
        va_end(ap);
    }
    time_t tnow;
    struct tm *tmnow;
    time(&tnow);
    tmnow = localtime(&tnow);

    //usec
    struct timeval tv;
    if(gettimeofday(&tv,NULL)<0)
        return 0;

    __android_log_print(ANDROID_LOG_ERROR,"HMI","%02d:%02d:%02d.%06d %s:%s():%d  %s",
                        tmnow->tm_hour,tmnow->tm_min,tmnow->tm_sec,tv.tv_usec,
                        file, func, line, msg);
}
#endif

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
class JniFrame : public QObject
{
    Q_OBJECT
public:
    explicit JniFrame(QObject *parent = 0);
    ~JniFrame();

    static void MsgNofityFromJni(int msgNo, int x, int y);
    void setAppList(AppListInterface *pList);
    void startStream();
    void stopStream();
private slots:

private:
    static AppListInterface *m_pList;
};

#endif // JNIFRAME_H
