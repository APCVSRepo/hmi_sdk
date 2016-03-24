#include "MspVRAudio.h"
#include "lib_msp_vr.h"
#include <QLibrary>
#include <QFile>
#include <qendian.h>
#include <QCoreApplication>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include "pthread.h"
#include "Config/Config.h"
#ifdef WIN32
#include <Windows.h>
#endif
#ifdef linux
#include <unistd.h>
#endif

void logprint(char buf[])
{
    LOGI(buf);
}

msp_vr_audio::msp_vr_audio()
{
    //msp set
#ifdef SDL_SUPPORT_VR
#if  (!RECORD_TEST)
    msp_mem_init(MSP_MEM_CLIENT);
    msp_vr_upload(false);
#endif

    msp_vr_register_log_callback(logprint);
    msp_log_mode(ASR_RUN_RELEASE);
#endif
    //parm set
    vr_type=MSP_VR_APP;
    max_duration=3000;//ms
    sampling_rate=8000;
    bits_per_sample=16;
    audio_type=1;
    //audio set
    audioDevice=new AudioInput(sampling_rate,bits_per_sample,audio_type,this);
    connect(audioDevice,SIGNAL(pushBuffer(bool,char*,int)),SLOT(handleProbBuffer(bool,char*,int)));

    //show device info
    LOGI("there is %d available audio input device",QAudioDeviceInfo::availableDevices(QAudio::AudioInput).count());
    if(QAudioDeviceInfo::availableDevices(QAudio::AudioInput).count()==0){
        LOGE("no available audio input devices");
        return;
    }
    foreach (const QAudioDeviceInfo &device, QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        LOGI("%s\n",device.deviceName().toStdString().data());
    }
    LOGI("select Default\n-----------------------------------------");
    QAudioDeviceInfo defaultDevice(QAudioDeviceInfo::defaultInputDevice());
    //audio devices
    LOGI("inputs:\n%s",defaultDevice.deviceName().toUtf8().data());


    //audio codecs
    LOGI("audio codec:\nDefault");
    foreach (const QString &codecName, defaultDevice.supportedCodecs()) {
        LOGI(codecName.toStdString().data());
    }
    LOGI("select Default\n-----------------------------------------");

    //containers
    LOGI("sampleSize:\nDefault");
    foreach (const int smpl_size, defaultDevice.supportedSampleSizes()) {
        LOGI("%dbit",smpl_size);
    }
    LOGI("select 16bit\n-----------------------------------------");

    LOGI(defaultDevice.preferredFormat().codec().toStdString().data());

    LOGI("sample rate:\nDefault 0");
    foreach (int sampleRate, defaultDevice.supportedSampleRates()) {
        LOGI("%d",sampleRate);
    }
    LOGI("select 8000Hz\n-----------------------------------------");

    counter=0;
#if  (!RECORD_TEST)
    pthread_t  pl;
    pthread_create(&pl,NULL,loopRecordThread,this);
#else
    record_start();
#endif
 }



void msp_vr_audio::audioFormat()
{
#ifdef SDL_SUPPORT_VR
    bool ret=msp_vrparm_get(&vr_type,&max_duration,&sampling_rate,&bits_per_sample,&audio_type);
    LOGI("ret=%d,vr_type=%d,max_duration=%d,sampling_rate=%d,bits_per_sample=%d,audio_type=%d\n",ret,
           vr_type,max_duration,sampling_rate,bits_per_sample,audio_type);
#endif
    audioDevice->setFormat(sampling_rate,bits_per_sample,audio_type);
}
void * msp_vr_audio::loopRecordThread(void *arg)
{
    msp_vr_audio *msp_handle=static_cast<msp_vr_audio*>(arg);
    if(msp_handle==NULL)
        return NULL;
    while(true){
        msp_handle->loopRecord();
#ifdef WIN32
        Sleep(200);//100ms
#else
        usleep(200*1000);//100ms
#endif
    }

}

//slots
void msp_vr_audio::loopRecord()
{
    int time = max_duration/100;
    int state =0;
#ifdef SDL_SUPPORT_VR
    state=msp_vr_state();
#endif
    counter++;
    if(state==MSP_RECORD_START)//指示开始录音
    {
        LOGI("state=%d",state);
#ifdef SDL_SUPPORT_VR
        msp_mem_data_init();
#endif
        counter=0;
        audioFormat();
#ifdef SDL_SUPPORT_VR
        msp_vr_set_state(MSP_RECORD_WAIT);
#endif
        record_start();

    }
    else if(state==MSP_RECORD_WAIT)
    {
        LOGI("state=%d",state);
        if(counter>=time)
        {
            record_end();
            counter=0;
        }
    }
    else if(state==MSP_RECORD_END)
    {
        if(audioDevice->state()==QAudio::ActiveState)
        {
//            audioRecorder->stop();
            record_end();
            counter=0;
        }
    }
    else if(state==MSP_RECORD_INIT||state==-1)
    {
       // LOGI("state=%d",state);
#ifdef SDL_SUPPORT_VR
        msp_mem_init(MSP_MEM_CLIENT);
        msp_vr_set_state(MSP_RECORD_END);
#endif
    }
    else if(state==MSP_RECORD_TERMINATE)
    {
        LOGI("state=%d",state);
        audioDevice->stop();
        counter=0;
    }
}



void msp_vr_audio::handleProbBuffer(bool isOver,char *buffer,int len)
{
#ifdef SDL_SUPPORT_VR
    if(vr_type==MSP_VR_APP){
       if(isOver)
          record_end();
#if  (!RECORD_TEST)
       else
           msp_vr_upload_data((char *)buffer,len);
#endif
    }
    else
    {
#if  (!RECORD_TEST)
        msp_vr_upload_data((char*)buffer,len);
#endif
    }
#endif
}


msp_vr_audio::~msp_vr_audio()
{
    audioDevice->stop();
    delete  audioDevice;
}

//slots
void msp_vr_audio::record_start()
{
    audioDevice->start();
    LOGI("record start");
}

void msp_vr_audio::record_end()
{
#if  (!RECORD_TEST)
#ifdef SDL_SUPPORT_VR
    msp_vr_set_state(MSP_RECORD_END);
#endif
#endif
    LOGI("record end recording");

    audioDevice->stop();
    LOGI("record stop");
}


