#include "VideoStream.h"
extern Config g_config;

#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
//#include <windows.h>


#include "Connect/SDLConnector.h"
using namespace std;



VideoStream::VideoStream(QWidget *parent) :
    QWidget(parent)
{
    m_i_w = g_config.m_i_mainWindowW;
    m_i_h = g_config.m_i_mainWindowH;

    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setGeometry(0,0,m_i_w,m_i_h);

    m_Screen=new QLabel(this);
    m_Screen->setGeometry(1,1,m_i_w,m_i_h);

    connect(this,SIGNAL(GetImage(QImage)),this,SLOT(SetImageSlots(QImage)));

    m_Stop=true;

    m_i_frameFinished = 0;
    m_streamOpenSucess=false;

}




VideoStream::~VideoStream()
{
    delete m_Screen;
}

void VideoStream::setUrl(QString url)
{
    m_str_url = url;
}

void VideoStream::av_log_default_callback(void* ptr, int level, const char* fmt, va_list vl)
{
    char fmtBuf[256]={0};
    vsprintf(fmtBuf,fmt,vl);
    va_end(vl);
    printf("level=%d:%s",level,fmtBuf);
    fflush(stdout);
}



void VideoStream::startStream()
{
    showTestLog("\nstartStream---xu");
#ifdef VIDEO_STREM_NET
    m_Stop=true;
   // m_timerPlay->stop();
    videoStreamIndex=-1;
    av_register_all();//注册库中所有可用的文件格式和解码器
    avcodec_register_all();
    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行
    pAVFormatContext = avformat_alloc_context();//申请一个AVFormatContext结构的内存,并进行简单初始化

    av_log_set_callback(VideoStream::av_log_default_callback);

    pAVFrame=av_frame_alloc();
    m_streamOpenSucess=false;

    if(this->Init()){
        m_Stop=false;
        pthread_t frame_read_;
        pthread_create(&frame_read_,NULL,VideoStream::ReadVideoFrame,this);
        this->show();
    }
    else{
        showTestLog("init failed");
    }

#endif
}


void* VideoStream::ReadVideoFrame(void *arg)
{
    VideoStream *h=(VideoStream*)arg;

    //一帧一帧读取视频
    h->PlayImageSlots();
    return NULL;
}

void VideoStream::stopStream()
{
    m_Stop=true;
    this->hide();
    #ifdef VIDEO_STREM_NET
    avformat_free_context(pAVFormatContext);
    av_frame_free(&pAVFrame);
    sws_freeContext(pSwsContext);
#endif
}


void VideoStream::SetImageSlots(const QImage &image)
{
    if (image.height()>0){
        QPixmap pix = QPixmap::fromImage(image);
       // m_label->setPixmap(pix);
        m_Screen->setPixmap(pix);
    }
}
void VideoStream::PlayImageSlots()
{
    while(m_Stop==false){
        if(av_read_frame(pAVFormatContext, &pAVPacket) >= 0){
            if(pAVPacket.stream_index==videoStreamIndex){
                avcodec_decode_video2(pAVCodecContext, pAVFrame, &m_i_frameFinished, &pAVPacket);
                if (m_i_frameFinished){
                    mutex.lock();
                    sws_scale(pSwsContext,(const uint8_t* const *)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVPicture.data,pAVPicture.linesize);

                    //发送获取一帧图像信号
                    QImage image=QImage(pAVPicture.data[0],videoWidth,videoHeight,QImage::Format_RGB888);
                    emit GetImage(image);
                    mutex.unlock();

                }
            }
        }
        av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
    }
}


bool VideoStream::Init()
{
    m_streamOpenSucess=false;
    if(m_str_url.isEmpty())
        return false;
#ifdef VIDEO_STREM_NET
    //打开视频流
    showTestLog("avformat_open_input");
    int result=avformat_open_input(&pAVFormatContext,"tcp://127.0.0.1:5050",NULL,NULL);//m_str_url.toUtf8().data()
    if (result<0){
//        qDebug()<<"打开视频流失败";
        char errbuf[1024]={0};
        int e=av_strerror(result,errbuf,1024);
        showTestLog("avformat_open_input failed,error=%d,%d,%s",result,e,errbuf);
        return false;
    }

    //获取视频流信息
#if 1
   pAVFormatContext->probesize = 100 *1024;
   pAVFormatContext->max_analyze_duration = 1 * AV_TIME_BASE;
#endif
   showTestLog("avformat_find_stream_info");
   result=avformat_find_stream_info(pAVFormatContext,NULL);
   if (result<0){
      showTestLog("获取视频流信息失败%d",3);
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
    showTestLog("pAVCodecContext");
    if (videoStreamIndex==-1){

        showTestLog("获取视频流索引失败,%d",2);
        return false;
    }

    //获取视频流的分辨率大小
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
   // pAVCodecContext->width=1333;
    videoWidth=800;//pAVCodecContext->width;
    videoHeight=480;//pAVCodecContext->height;

    QRect rect=this->geometry();
    int left=(rect.width()-videoWidth)/2;
    int up=(rect.height()-videoHeight)/2;
    m_Screen->setGeometry(left,up,videoWidth,videoHeight);

    showTestLog("Screen resolution:width:%d,height:%d",videoWidth,videoHeight);
    avpicture_alloc(&pAVPicture,PIX_FMT_RGB24,videoWidth,videoHeight);

    AVCodec *pAVCodec;

    //获取视频流解码器
    showTestLog("avcodec_find_decoder");
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
    showTestLog("sws_getContext");
    pSwsContext = sws_getContext(videoWidth,videoHeight,PIX_FMT_YUV420P,videoWidth,videoHeight,PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);

    showTestLog("avcodec_open2");
    //打开对应解码器
    result=avcodec_open2(pAVCodecContext,pAVCodec,NULL);
    if (result<0){
        showTestLog("打开解码器失败,%d",1);
        return false;
    }
#endif
    m_Stop=false;
    m_streamOpenSucess=true;
//    qDebug()<<"初始化视频流成功";
    return true;
}


void VideoStream::mousePressEvent(QMouseEvent *e)
{
    int w=this->width();
    int h=this->height();
    int x=e->x();
    int y=e->y();
    showTestLog("Touch Event:type=BEGIN,x=%d,y=%d",x,y);
    SDLConnector::getSDLConnectore()->OnVideoScreenTouch(TOUCH_START,x,y);
}


void VideoStream::mouseMoveEvent(QMouseEvent *e)
{
    int w=this->width();
    int h=this->height();
    int x=e->x();
    int y=e->y();
    showTestLog("Touch Event:type=MOVE,x=%d,y=%d",x,y);

    SDLConnector::getSDLConnectore()->OnVideoScreenTouch(TOUCH_MOVE,x,y);
}

void VideoStream::mouseReleaseEvent(QMouseEvent *e)
{
    int w=this->width();
    int h=this->height();
    int x=e->x();
    int y=e->y();
    showTestLog("Touch Event:type=END,x=%d,y=%d",x,y);

    SDLConnector::getSDLConnectore()->OnVideoScreenTouch(TOUCH_END,x,y);
}


void VideoStream::showTestLog(char *format,...)
{
    char ifbuf[300]={0};
    va_list  ap;
   //    int retval;
    va_start(ap,format);
    vsprintf(ifbuf,format,ap);
    va_end(ap);
    strcat(ifbuf,"\n");

   // m_info->setText(ifbuf);
   // std::cout<<ifbuf;
    printf(ifbuf);
    fflush(stdout);
}

