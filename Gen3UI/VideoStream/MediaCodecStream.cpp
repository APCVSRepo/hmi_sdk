#include "MediaCodecStream.h"

#ifdef PLAY_FILE_TEST
uchar readBuf[1024] = {0};
#endif

MediaCodecStream::MediaCodecStream(AppListInterface * pList, QWidget *parent) : QWidget(parent)
{
    m_jniFrame.setAppList(pList);

    setWindowFlags(Qt::FramelessWindowHint);
    if(parent)
    {
        setGeometry(0,0,parent->width(),parent->height());
    }

#ifdef PLAY_FILE_TEST
    fp = fopen("/mnt/sdcard/gaode.mp4", "rb");
#endif

}

MediaCodecStream::~MediaCodecStream()
{
#ifdef PLAY_FILE_TEST
    fclose(fp);
#endif
}

void MediaCodecStream::showActivity()
{
    m_b_canPushQueue = true;
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

//static int findFirstFrame = 0;
//static int byteCount = 4;
//#define WIDTH_DECODER 800
//#define HEIGHT_DECODER  480
//static uchar frameData[WIDTH_DECODER * HEIGHT_DECODER * 3 / 2] = {0};
//static int lastIndex = 0;
void MediaCodecStream::startStream()
{
#ifdef ANDROID
#ifdef PLAY_FILE_TEST
    fseek(fp,0L,SEEK_SET);
#endif

    QAndroidJniObject::callStaticMethod<void>(
                "an/qt/useJar/ExtendsQtSurface",
                "start","()V");
//    findFirstFrame = 0;
//    byteCount = 4;
//    lastIndex = 0;

#endif
}

void MediaCodecStream::stopStream()
{
#if defined(ANDROID)
    m_jniFrame.stopStream();
//    m_b_canPushQueue = false;

//    findFirstFrame = 0;
//    byteCount = 4;
//    lastIndex = 0;
#endif
}

void MediaCodecStream::onRawData(void* data, int len)
{
#ifdef PLAY_FILE_TEST
    fread(readBuf,len,1,fp);
    data = readBuf;
#endif
//    if(!m_b_canPushQueue)
//    {
//        return;
//    }

////    __D("len ===== %d\n", len);
//    frameData[0] = 0;
//    frameData[1] = 0;
//    frameData[2] = 0;
//    frameData[3] = 1;
//    int nPos = 0;
//    int index = 0;
//    int flag = 0;
//    uchar *buffer = (uchar*)data;

//    int canOnFrame = 0;
//    while (nPos < len)
//    {
//        canOnFrame = 0;
//        while (nPos < len)
//        {
//            flag = buffer[nPos++];
//            if(findFirstFrame == 1)
//            {
//                frameData[byteCount++] = (uchar)flag;
//            }
//            if (flag == 0)
//            {
//                index = lastIndex + 1;
//                while (flag == 0)
//                {
//                    if (nPos < len)
//                    {
//                        lastIndex = 0;
//                        flag = buffer[nPos++];
//                        if(findFirstFrame == 1)
//                        {
//                            frameData[byteCount++] = (uchar)flag;
//                        }
//                        index++;
//                    }
//                    else
//                    {
//                        lastIndex = index;
//                        break;
//                    }
//                }
//                if (flag == 1 && index >= 4)
//                {
//                    if(findFirstFrame == 0)
//                        findFirstFrame = 1;
//                    else
//                    {
//                        byteCount = byteCount - 4;
//                        canOnFrame = 1;
//                    }

//                    break;
//                }
//            }
//        }
//        if(canOnFrame == 1)
//        {
////            __D("\n");
//            FrameS newFrame;
//            memcpy(newFrame.buf, frameData, byteCount);
//            newFrame.len = byteCount;
//            m_jniFrame.pushFrameQueue(newFrame);

//            byteCount = 4;
//        }
//    }

}


