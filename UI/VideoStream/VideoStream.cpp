#include "VideoStream.h"
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
//#include <windows.h>
#include <QMouseEvent>
#include <QPainter>
#include "Include/global_first.h"

#include "Connect/SDLConnector.h"
#include "main.h"
using namespace std;
#ifdef VIDEO_STREAM_MEM
//char * video_buffer[AV_BUFFER_SIZE];
//int    video_start,video_end;//[)
QByteArray  arrayBuffer;
pthread_mutex_t  _mutex_video_buffer;
#endif
VideoStream::VideoStream(int w,int h,QWidget *parent) :
#ifdef VIDEO_STREAM_WIDGET
    QVideoWidget(parent)
#else
    QWidget(parent),pAVFormatContext(nullptr),pAVFrame(nullptr),pSwsContext(nullptr)
#endif
{
    m_i_w = w;
    m_i_h = h;

    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setGeometry(0,0,m_i_w,m_i_h);

#ifdef VIDEO_STREAM_WIDGET
    m_VideoPlayer=new QMediaPlayer;
    m_VideoPlayer->setVideoOutput(this);

#else
    av_register_all();//注册库中所有可用的文件格式和解码器
    avcodec_register_all();
    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行


#ifdef VIDEO_STREAM_MEM
    sdl_set_videostream_callback(callBack_send_data);
//    video_start=video_end=0;
    arrayBuffer.clear();
    pthread_mutex_init(&_mutex_video_buffer,NULL);
#endif

    isGetPacket=false;
    m_Stop=true;

    m_i_frameFinished = 0;
#endif

}




VideoStream::~VideoStream()
{

#ifdef VIDEO_STREAM_WIDGET
    delete m_VideoPlayer;
#else
    if(pAVFormatContext != nullptr)
    {
        avformat_free_context(pAVFormatContext);
    }
    if(pAVFrame != nullptr)
    {
        av_frame_free(&pAVFrame);
    }
    if(pSwsContext != nullptr)
    {
        sws_freeContext(pSwsContext);
    }
    //delete m_Screen;
#endif

}

void VideoStream::setUrl(QString url)
{
    LOGI("setUrl");
    LOGI("url:%s",url.toUtf8().data());
    m_str_url = url;
#ifdef VIDEO_STREAM_WIDGET
    m_VideoPlayer->setMedia(QMediaContent(QNetworkRequest(QUrl(url))));
#endif
}

void VideoStream::av_log_default_callback(void* ptr, int level, const char* fmt, va_list vl)
{
    char fmtBuf[256]={0};
    vsprintf(fmtBuf,fmt,vl);
    va_end(vl);
    LOGD("level=%d:%s",level,fmtBuf);
}

#ifdef VIDEO_STREAM_MEM

void VideoStream::callBack_send_data(const char *data, int size)
{
   // LOGI("callback size=%d\n",size);
    arrayBuffer.append(data,size);
//    pthread_mutex_lock(&_mutex_video_buffer);
//    int space=(video_start+AV_BUFFER_SIZE-video_end);
//    if(space>=size){
//        if(AV_BUFFER_SIZE-)
//        memcpy(&video_buffer[video_end],data,size);
//        video_end +=size;
//    }

//    pthread_mutex_unlock(&_mutex_video_buffer);
}

int VideoStream::read_buffer(void *opaque,uint8_t *buf,int buf_size)
{
    int size=arrayBuffer.size();
    if(size>=buf_size){
        memcpy(buf,arrayBuffer.data(),buf_size);
        arrayBuffer.remove(0,buf_size);
        return buf_size;
    }
    else{
        memcpy(buf,arrayBuffer.data(),arrayBuffer.size());
        arrayBuffer.clear();
        return size;
    }
}
#endif


#ifdef VIDEO_STREAM_WIDGET
void VideoStream::startStream()
{
    LOGD("\n startStream");
    this->show();

    m_VideoPlayer->play();
}

#else
void VideoStream::startStream()
{
    LOGD("\n startStream");
    m_Stop=true;
   // m_timerPlay->stop();
    videoStreamIndex=-1;

    pAVFormatContext = avformat_alloc_context();//申请一个AVFormatContext结构的内存,并进行简单初始化
#ifdef VIDEO_STREAM_MEM
//    video_start=video_end=0;
    arrayBuffer.clear();
    unsigned char * aviobuffer = (unsigned char *)av_malloc(AV_BUFFER_SIZE);
    AVIOContext * avioContext = avio_alloc_context(aviobuffer,AV_BUFFER_SIZE,0,this,VideoStream::read_buffer,NULL,NULL);
    pAVFormatContext->pb=avioContext;
#endif
    av_log_set_callback(VideoStream::av_log_default_callback);

    pAVFrame=av_frame_alloc();

    if(this->Init()){
        m_Stop=false;
        this->show();
        pthread_t frame_read_;
        pthread_create(&frame_read_,NULL,VideoStream::ReadVideoFrame,this);

    }
    else{
        LOGD("init failed");
    }
}

bool VideoStream::Init()
{
    if(m_str_url.isEmpty())
        return false;
    //打开视频流
    LOGD("avformat_open_input");

#ifdef VIDEO_STREAM_MEM
    int result=avformat_open_input(&pAVFormatContext,NULL,NULL,NULL);
#else
    int result=avformat_open_input(&pAVFormatContext,m_str_url.toUtf8().data(),NULL,NULL);//"tcp://127.0.0.1:5050"
#endif
    if (result<0){
        char errbuf[1024]={0};
        int e=av_strerror(result,errbuf,1024);
        LOGD("avformat_open_input failed,error=%d,%d,%s",result,e,errbuf);
        return false;
    }

    //获取视频流信息
#if 1
   pAVFormatContext->probesize = 100 *1024;
   pAVFormatContext->max_analyze_duration = 5 * AV_TIME_BASE;
#endif
   LOGD("avformat_find_stream_info");
   result=avformat_find_stream_info(pAVFormatContext,NULL);
   if (result<0){
      LOGD("获取视频流信息失败%d",3);
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
    LOGD("pAVCodecContext");
    if (videoStreamIndex==-1){

        LOGD("获取视频流索引失败,%d",2);
        return false;
    }

    //获取视频流的分辨率大小
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
   // pAVCodecContext->width=1333;
    videoWidth=pAVCodecContext->width;
    videoHeight=pAVCodecContext->height;
    if(videoWidth*videoHeight==0){
        videoWidth=800;
        videoHeight=480;
    }

    LOGD("Screen resolution:width:%d,height:%d",videoWidth,videoHeight);
    avpicture_alloc(&pAVPicture,AV_PIX_FMT_RGBA,videoWidth,videoHeight);

    AVCodec *pAVCodec;
    //获取视频流解码器
    LOGD("avcodec_find_decoder");
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
    LOGD("sws_getContext");
    pSwsContext = sws_getContext(videoWidth,videoHeight,PIX_FMT_YUV420P,videoWidth,videoHeight,AV_PIX_FMT_RGBA,SWS_BICUBIC,0,0,0);

    LOGD("avcodec_open2");
    //打开对应解码器
    result=avcodec_open2(pAVCodecContext,pAVCodec,NULL);
    if (result<0){
        LOGD("打开解码器失败,%d",1);
        return false;
    }

    m_Stop=false;
    return true;
}


void* VideoStream::ReadVideoFrame(void *arg)
{
    VideoStream *h=(VideoStream*)arg;

    //一帧一帧读取视频
    h->PlayImageSlots();
    return NULL;
}
#endif
void VideoStream::stopStream()
{
#ifdef VIDEO_STREAM_WIDGET
    m_VideoPlayer->stop();
#endif
#ifdef VIDEO_STREM_NET
    LOGI("stop stream:%d",(int)m_Stop);
    if(m_Stop)
        return;
    LOGI("avcodec_close");
    m_Stop=true;
    isGetPacket=false;
    avcodec_close(pAVCodecContext);
    LOGI("avformat_close_input");
    avformat_close_input(&pAVFormatContext);
    LOGI("avformat_free_context");
    avformat_free_context(pAVFormatContext);
    LOGI("av_frame_free");
    av_frame_free(&pAVFrame);
    LOGI("sws_freeContext");
    sws_freeContext(pSwsContext);
#endif
    this->hide();
}

#ifndef VIDEO_STREAM_WIDGET
void VideoStream::PlayImageSlots()
{
    while(m_Stop==false){
        AVPacket pAVPacket;
        if(av_read_frame(pAVFormatContext, &pAVPacket) == 0){
            //LOGD("data size=%d,packet num=%d,total_size=%d\n",pAVPacket.size,packet_num++,data_size+=pAVPacket.size);
            if(pAVPacket.stream_index==videoStreamIndex){
                avcodec_decode_video2(pAVCodecContext, pAVFrame, &m_i_frameFinished, &pAVPacket);
                if(m_i_frameFinished){
                    sws_scale(pSwsContext,(const uint8_t* const *)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVPicture.data,pAVPicture.linesize);
                    //发送获取一帧图像信号
                    m_VideoImage=QImage(pAVPicture.data[0],videoWidth,videoHeight,QImage::Format_RGBA8888);
                    //m_VideoImage=QImage(pAVFrame->data[0],pAVFrame->width,pAVFrame->height,QImage::Format_RGB888);
                    update();
                }
            }
            av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
        }
    }
}




void VideoStream::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    if(m_VideoImage.isNull())
        return;
    QPainter painter(this);
   // painter.drawImage(0,0,QImage(pAVPicture.data[0],width(),height(),QImage::Format_RGB888));
    painter.drawImage(QRect(0,0,this->width(),this->height()),m_VideoImage,QRect(0,0,m_VideoImage.width(),m_VideoImage.height()));
}
#endif


void VideoStream::mousePressEvent(QMouseEvent *e)
{
    int x=e->x();
    int y=e->y();
    x= x*videoWidth/width();
    y= y*videoHeight/height();
    LOGD("Touch Event:type=BEGIN,x=%d,y=%d",x,y);
    ToSDL->OnVideoScreenTouch(TOUCH_START,x,y);
}


void VideoStream::mouseMoveEvent(QMouseEvent *e)
{
    int x=e->x();
    int y=e->y();
    x= x*videoWidth/width();
    y= y*videoHeight/height();
    LOGD("Touch Event:type=MOVE,x=%d,y=%d",x,y);

    ToSDL->OnVideoScreenTouch(TOUCH_MOVE,x,y);
}

void VideoStream::mouseReleaseEvent(QMouseEvent *e)
{
    int x=e->x();
    int y=e->y();
    x= x*videoWidth/width();
    y= y*videoHeight/height();
    LOGD("Touch Event:type=END,x=%d,y=%d",x,y);

    ToSDL->OnVideoScreenTouch(TOUCH_END,x,y);
}



