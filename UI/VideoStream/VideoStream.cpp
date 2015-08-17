#include "VideoStream.h"
extern Config g_config;

#include <stdio.h>
#include "HMISDK/include/SDLConnector.h"

VideoStream::VideoStream(QWidget *parent) :
    QWidget(parent)
{
    m_i_w = g_config.m_i_mainWindowW;
    m_i_h = g_config.m_i_mainWindowH;

    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setGeometry(0,0,m_i_w,m_i_h);

    m_label = new QLabel(this);
    m_label->setGeometry(0,0,m_i_w,m_i_h);

//    m_label->installEventFilter(this);


    connect(this,SIGNAL(GetImage(QImage)),this,SLOT(SetImageSlots(QImage)));


    m_timerPlay = new QTimer;
    m_timerPlay->setInterval(10);
    connect(m_timerPlay,SIGNAL(timeout()),this,SLOT(playSlots()));

    m_i_frameFinished = 0;
}

VideoStream::~VideoStream()
{
    delete m_label;
    delete m_timerPlay;
}

void VideoStream::setUrl(QString url)
{
    m_str_url = url;
}

void VideoStream::startStream()
{
    videoStreamIndex=-1;
    av_register_all();//注册库中所有可用的文件格式和解码器
    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行
    pAVFormatContext = avformat_alloc_context();//申请一个AVFormatContext结构的内存,并进行简单初始化
    pAVFrame=av_frame_alloc();
    if (this->Init()){

        m_timerPlay->start();
    }
    this->show();
}
void VideoStream::stopStream()
{
    m_timerPlay->stop();

    avformat_free_context(pAVFormatContext);
    av_frame_free(&pAVFrame);
    sws_freeContext(pSwsContext);
}

void VideoStream::SetImageSlots(const QImage &image)
{
    if (image.height()>0){
        QPixmap pix = QPixmap::fromImage(image.scaled(m_i_w,m_i_h));
        m_label->setPixmap(pix);
    }
}


bool VideoStream::Init()
{
    if(m_str_url.isEmpty())
        return false;
    //打开视频流

    int result=avformat_open_input(&pAVFormatContext, m_str_url.toUtf8().data(),NULL,NULL);
    if (result<0){
//        qDebug()<<"打开视频流失败";
        return false;
    }

    //获取视频流信息

    pAVFormatContext->probesize = 100 *1024;
    pAVFormatContext->max_analyze_duration = 1 * AV_TIME_BASE;
//    pAVFormatContext->probesize = 2 *1024;
//    pAVFormatContext->max_analyze_duration = 1000;

    result=avformat_find_stream_info(pAVFormatContext,NULL);
    if (result<0){
//        qDebug()<<"获取视频流信息失败";
        return false;
    }

    //获取视频流索引
    videoStreamIndex = -1;
    for (uint i = 0; i < pAVFormatContext->nb_streams; i++) {
        if (pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex==-1){
//        qDebug()<<"获取视频流索引失败";
        return false;
    }

    //获取视频流的分辨率大小
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
    videoWidth=pAVCodecContext->width;
    videoHeight=pAVCodecContext->height;

    avpicture_alloc(&pAVPicture,PIX_FMT_RGB24,videoWidth,videoHeight);

    AVCodec *pAVCodec;

    //获取视频流解码器
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);

    pSwsContext = sws_getContext(videoWidth,videoHeight,PIX_FMT_YUV420P,videoWidth,videoHeight,PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);

    //打开对应解码器
    result=avcodec_open2(pAVCodecContext,pAVCodec,NULL);
    if (result<0){
//        qDebug()<<"打开解码器失败";
        return false;
    }

//    qDebug()<<"初始化视频流成功";
    return true;
}

void VideoStream::playSlots()
{
    //一帧一帧读取视频
    if (av_read_frame(pAVFormatContext, &pAVPacket) >= 0){
        if(pAVPacket.stream_index==videoStreamIndex){
//            qDebug()<<"开始解码"<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            avcodec_decode_video2(pAVCodecContext, pAVFrame, &m_i_frameFinished, &pAVPacket);
            if (m_i_frameFinished){
                mutex.lock();
                sws_scale(pSwsContext,(const uint8_t* const *)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVPicture.data,pAVPicture.linesize);
                //发送获取一帧图像信号
                QImage image(pAVPicture.data[0],videoWidth,videoHeight,QImage::Format_RGB888);
                emit GetImage(image);
                mutex.unlock();
            }
        }
    }
    av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
}


void VideoStream::mousePressEvent(QMouseEvent *e)
{
    SDLConnector::getSDLConnectore()->OnVideoScreenTouch(TOUCH_START,e->x(),e->y());
}


void VideoStream::mouseMoveEvent(QMouseEvent *e)
{
    SDLConnector::getSDLConnectore()->OnVideoScreenTouch(TOUCH_MOVE,e->x(),e->y());
}

void VideoStream::mouseReleaseEvent(QMouseEvent *e)
{
    SDLConnector::getSDLConnectore()->OnVideoScreenTouch(TOUCH_END,e->x(),e->y());
}
