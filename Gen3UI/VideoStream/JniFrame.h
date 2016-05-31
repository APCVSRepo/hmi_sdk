#ifndef JNIFRAME_H
#define JNIFRAME_H

#include <QObject>
#if defined(ANDROID)
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <jni.h>
#endif
#include <QTimer>
#include <QQueue>
#include <QMutex>
#include <unistd.h>
#include "JniNative.h"
#include "AppListInterface.h"
#define QDBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__

#define TIME_INTER 20
typedef struct FrameData{
    uchar buf[576000];
    int len;
}FrameS;
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
    void pushFrameQueue(FrameS &frameData);
    void clearFrameQueue();
    void stopStream();
private slots:

private:
    static AppListInterface *m_pList;
    static bool m_b_canFlush;

    static QQueue<FrameS> m_queueFrame;
    static QMutex m_mutex;
    pthread_t threadid;
    static void *flushDataThread(void *args);
};

#endif // JNIFRAME_H
