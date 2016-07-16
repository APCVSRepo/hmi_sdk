#ifndef MEDIACODECSTREAM_H
#define MEDIACODECSTREAM_H

#include <QWidget>
#if defined(ANDROID)
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
using namespace QtAndroid;
#include <unistd.h>
#endif
#include <QPainter>
#include <QQueue>
#include <QDebug>
#include "../hmi_sdk/HMI_SDK/Connect/IMessageInterface.h"
#include <QMutex>
#include "JniNative.h"

#include "Common/Button.h"
#include "AppListInterface.h"
#include "JniFrame.h"

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
    Result onRequest(Json::Value) {return RESULT_SUCCESS;}
    void onNotification(Json::Value) {}
    void onResult(Json::Value) {}
    void onRawData(void * p, int iLength) {}
    void onError(std::string error) {}
signals:

public slots:

private:
    JniFrame m_jniFrame;

#ifdef SDL_CALL_BACK
    static void callBack_send_data(const char *data, int size);
#endif
};

#endif // MEDIACODECSTREAM_H
