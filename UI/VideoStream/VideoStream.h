#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <QWidget>
#include "UI/Config/Config.h"
#include <QLabel>

////引入ffmpeg头文件
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#ifdef __cplusplus
}
#endif

#include <QObject>
#include <QMutex>
#include <QImage>
#include <QTcpSocket>
#include <QByteArray>
#include <QMutex>

#ifdef VIDEO_STREAM_WIDGET
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#endif

#ifndef VIDEO_STREAM_WIDGET
//#define     VIDEO_STREAM_FILE    // 2//1:NET  2:FILE
#define     VIDEO_STREM_NET
//#define     VIDEO_STREAM_MEM
#define     AV_BUFFER_SIZE    (1024*1024)
#endif

class VideoStream :
        #ifdef VIDEO_STREAM_WIDGET
        public QVideoWidget
        #else
        public QWidget
        #endif
{
    Q_OBJECT
public:
    explicit VideoStream(int w,int h,QWidget *parent = 0);
    ~VideoStream();
    
    void setUrl(QString url);
    void startStream();
    void stopStream();
#ifdef VIDEO_STREM_MEM
    static void callBack_send_data(const char *data,int size);
    static int read_buffer(void *opaque,uint8_t *buf,int buf_size);
#endif
    static void av_log_default_callback(void* ptr, int level, const char* fmt, va_list vl);
#ifndef VIDEO_STREAM_WIDGET
    static void* ReadVideoFrame(void* arg);
    void paintEvent(QPaintEvent *e);
#endif
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

#ifndef VIDEO_STREAM_WIDGET
signals:
    void GetImage(const QImage &image); 
public slots:
    void PlayImageSlots();
#endif
private:
    int m_i_w;
    int m_i_h;
    QString m_str_url;
    int videoWidth;
    int videoHeight;
#ifdef VIDEO_STREAM_WIDGET
    QMediaPlayer  *m_VideoPlayer;
#else
  //  QLabel *m_label;
    QTimer *m_timerPlay;
    QImage m_image;
    bool   m_Stop;
   // QLabel  *m_Screen;
    QImage    m_VideoImage;
    QMutex mutex;
    AVPicture  pAVPicture;
    AVFormatContext *pAVFormatContext;
    AVCodecContext *pAVCodecContext;
#ifdef VIDEO_STREAM_MEM
    AVIOContext  *pAVIOContext;
#endif

    AVFrame *pAVFrame;
    SwsContext * pSwsContext;

    bool     isGetPacket;


    int videoStreamIndex;
    int m_i_frameFinished;

    bool Init();
#endif
    
};

#endif // VIDEOSTREAM_H
