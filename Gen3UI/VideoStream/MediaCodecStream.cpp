#include "MediaCodecStream.h"
#include "main.h"
MediaCodecStream::MediaCodecStream(AppListInterface * pList, QWidget *parent) : QWidget(parent)
{
    m_jniFrame.setAppList(pList);

    setWindowFlags(Qt::FramelessWindowHint);
    if (parent) {
        setGeometry(0,0,parent->width(),parent->height());
    }

#ifdef SDL_CALL_BACK
    sdl_set_videostream_callback(callBack_send_data);
#endif
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
#ifndef SDL_CALL_BACK
    m_jniFrame.startStream();
#endif
}

void MediaCodecStream::stopStream()
{
    m_jniFrame.stopStream();
}

#ifdef SDL_CALL_BACK
#define TMP_BUF_LEN 100
static uchar m_tmpBuf[TMP_BUF_LEN + 2048] = {0};
static int offset = 0;
void MediaCodecStream::callBack_send_data(const char *data, int size)
{
#ifdef ANDROID
    if (size + offset > TMP_BUF_LEN) {
        if (JniNative::gBuffer != NULL) {
            memcpy(m_tmpBuf + offset, data, size);
            memcpy(JniNative::gBuffer, m_tmpBuf, size + offset);
            QAndroidJniObject::callStaticMethod<void>(
                        "an/qt/useJar/ExtendsQtSurface",
                        "flsh", "(I)V", size + offset);
            offset = 0;
        }
    } else {
        memcpy(m_tmpBuf + offset, data, size);
        offset += size;
    }
#endif
}
#endif
