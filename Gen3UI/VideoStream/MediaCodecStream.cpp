#include "MediaCodecStream.h"

MediaCodecStream::MediaCodecStream(AppListInterface * pList, QWidget *parent) : QWidget(parent)
{
    m_jniFrame.setAppList(pList);

    setWindowFlags(Qt::FramelessWindowHint);
    if(parent)
    {
        setGeometry(0,0,parent->width(),parent->height());
    }
}

MediaCodecStream::~MediaCodecStream()
{
}

void MediaCodecStream::showActivity()
{
#ifdef ANDROID
    QAndroidJniObject action = QAndroidJniObject::fromString("intent_Extends_Qt_Surface");
    QAndroidJniObject intent("android/content/Intent",
                             "(Ljava/lang/String;)V",
                             action.object<jstring>());
    startActivity(intent, 0);
#endif
}

void MediaCodecStream::hideActivity()
{
    stopStream();
}

void MediaCodecStream::startStream()
{
#ifdef ANDROID
    m_jniFrame.startStream();
#endif
}

void MediaCodecStream::stopStream()
{
#if defined(ANDROID)
    m_jniFrame.stopStream();
#endif
}
