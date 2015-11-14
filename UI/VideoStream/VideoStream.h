#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <QWidget>
#include "UI/Config/Config.h"


//必须加以下内容,否则编译不能通过,为了兼容C和C99标准
#ifndef INT64_C
#define INT64_C
#define UINT64_C
#endif

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

//#define     VIDEO_STREAM_FILE    // 2//1:NET  2:FILE
#define     VIDEO_STREM_NET
//#define     VIDEO_STREAM_PIPE
class VideoStream : public QWidget
{
    Q_OBJECT
public:
    explicit VideoStream(QWidget *parent = 0);
    ~VideoStream();
    
    void setUrl(QString url);
    void startStream();
    void stopStream();
    static void av_log_default_callback(void* ptr, int level, const char* fmt, va_list vl);

    static void* ReadVideoFrame(void* arg);

    void showTestLog(char *info,...);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

signals:
    void GetImage(const QImage &image);
    
public slots:
    void SetImageSlots(const QImage &image);
    void PlayImageSlots();
private:

  //  QLabel *m_label;
    int m_i_w;
    int m_i_h;
    QString m_str_url;
    QTimer *m_timerPlay;
    QImage m_image;
    bool   m_Stop;
    bool   m_streamOpenSucess;
    QLabel  *m_Screen;
    QByteArray  videobuffer;
    QMutex mutex;
    AVPicture  pAVPicture;
    AVFormatContext *pAVFormatContext;
    AVCodecContext *pAVCodecContext;

    AVFrame *pAVFrame;
    SwsContext * pSwsContext;
    AVPacket pAVPacket;

    int videoWidth;
    int videoHeight;
    int videoStreamIndex;
    int m_i_frameFinished;

    bool Init();
    
};

#endif // VIDEOSTREAM_H
