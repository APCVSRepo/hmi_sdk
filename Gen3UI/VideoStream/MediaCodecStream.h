#ifndef MEDIACODECSTREAM_H
#define MEDIACODECSTREAM_H

#include <QWidget>
#if defined(ANDROID)
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
using namespace QtAndroid;
#endif
#include <QPainter>
#include <QQueue>
#include <QDebug>
#include "../hmi_sdk/HMI_SDK/Connect/IMessageInterface.h"
#include <QMutex>
#include "JniNative.h"
#include <unistd.h>
#include "Common/Button.h"
#include "AppListInterface.h"
#include "JniFrame.h"

//#define PLAY_FILE_TEST
#define FLUSH_FRAME

typedef struct dataPackage{
    uchar buf[1024];
    int len;
}DataS;
//typedef struct FrameData{
//    uchar buf[576000];
//    int len;
//}FrameS;

class MediaCodecStream : public QWidget, public IMessageInterface
{
    Q_OBJECT
public:
    explicit MediaCodecStream(AppListInterface * pList,QWidget *parent = 0);
    ~MediaCodecStream();

    void showActivity();
    void hideActivity();
    void startStream();
    void stopStream();

public: //IMessageInterface
    Result onRequest(Json::Value);
    void onNotification(Json::Value);
    void onResult(Json::Value);
    void onRawData(void * p, int iLength);
    void onError(std::string error);
signals:

public slots:

private:
    QImage m_VideoImage;

    QTimer *m_pTimer;

    QTimer *m_playTimer;

    QLabel *m_pVideoLab;

    pthread_t threadid;
    pthread_t msgThreadid;
#ifdef PLAY_FILE_TEST
    FILE *fp;
#endif
    bool m_b_canPushQueue;

    JniFrame m_jniFrame;
};

#endif // MEDIACODECSTREAM_H
