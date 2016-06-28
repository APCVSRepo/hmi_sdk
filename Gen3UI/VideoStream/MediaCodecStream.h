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

typedef struct dataPackage{
    uchar buf[1024];
    int len;
}DataS;

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
    Result onRequest(Json::Value){return RESULT_SUCCESS;}
    void onNotification(Json::Value){}
    void onResult(Json::Value){}
    void onRawData(void * p, int iLength){}
    void onError(std::string error){}
signals:

public slots:

private:
    JniFrame m_jniFrame;
};

#endif // MEDIACODECSTREAM_H
