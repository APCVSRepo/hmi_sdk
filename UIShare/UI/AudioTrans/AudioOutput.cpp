#include "AudioOutput.h"
#include "Config/Config.h"
#include <qendian.h>
//#include <unistd.h>

AudioOutput::AudioOutput(int smpl_rate,int smpl_size,QObject *parent):QObject(parent)
{
    audioOut=NULL;
    audioFormat.setSampleRate(smpl_rate);
    audioFormat.setChannelCount(1);
    audioFormat.setCodec("audio/pcm");
    audioFormat.setSampleType(QAudioFormat::SignedInt);
    audioFormat.setSampleSize(smpl_size);
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    deviceInfo=QAudioDeviceInfo::defaultOutputDevice();
    //restartAudioOutDevice();
   // this->startTimer(200);
   // soxr_wav_convert_init(smpl_rate,smpl_rate,smpl_size);
}

AudioOutput::~AudioOutput()
{
    //delete deviceInfo;
    //delete audioDevice;
    //delete audioOut;
    //soxr_wav_convert_destroy();
    this->CloseAudio();
}

void AudioOutput::getByteArray(char *data,int &length)
{
    const int channelBytes = audioFormat.sampleSize() / 8;
    short *ptr = reinterpret_cast<short *>(data);
    char *dptr = reinterpret_cast<char *>(data);
    //LOGI(data);
    for(int i=0;i<length/channelBytes;i++)
    {
        if(audioFormat.sampleSize()==16){
            dptr[i]=ptr[i];//*(128.0/32786.0);//qFromLittleEndian<qint16>(&ptr[i*channelBytes]);
            LOGI("dptr[%d]=%d",i,dptr[i]);
        }
    }
    length=length/channelBytes;
}



bool AudioOutput::restartAudioOutDevice()
{
    if(deviceInfo.isNull()){
        LOGE("there is no availiable device");
        return false;
    }
    LOGI("default audio output device:%s",deviceInfo.deviceName().toUtf8().data());
    if(!deviceInfo.isFormatSupported(audioFormat)){
        audioFormat=deviceInfo.nearestFormat(audioFormat);
    }
    LOGI("audio output audioFormat: smpl rate=%d,smpl size=%d,chn=%d,codec=%s",
         audioFormat.sampleRate(),audioFormat.sampleSize(),audioFormat.channelCount(),
         audioFormat.codec().toUtf8().data());
    if(audioOut!=NULL){
        audioOut->stop();
        delete audioOut;
    }
    audioOut=new QAudioOutput(deviceInfo,audioFormat);
    audioDevice=audioOut->start();
    if(audioDevice==NULL){
        LOGE("audioOut start failed");
        return false;
    }

    return true;
}


void AudioOutput::OpenAudio()
{
    //SetSmplRate(rate);
    //SetSmplSmplSize(size);
    restartAudioOutDevice();
}

void AudioOutput::CloseAudio()
{
    if(audioOut!=NULL){
        audioOut->stop();
        delete audioOut;
        audioOut=NULL;
    }
}


bool AudioOutput::Speak(char *data,int length)
{
    LOGI("AudioOuput::Speak,len=%d",length);
    int index=0;
    int left=length-index;
    while(left>=audioOut->bytesFree()){
        int chunks=audioOut->bytesFree()/(audioOut->periodSize());
        while (chunks) {
           int len=0;
           if(left<audioOut->periodSize()){
               len=audioDevice->write(&data[index],audioOut->periodSize());
               index +=audioOut->periodSize();
               left=length-index;
               break;
           }
           else{
               len=audioDevice->write(&data[index],audioOut->periodSize());
               index +=audioOut->periodSize();
               left=length-index;
           }
           LOGI("periodSize=%d,actual=%d,left=%d",audioOut->periodSize(),len,left);
           --chunks;
        }
    }
    return true;
}

void AudioOutput::SetSmplRate(int smpl_rate)
{
    audioFormat.setSampleRate(smpl_rate);
}

void AudioOutput::SetSmplSmplSize(int smpl_size)
{
    audioFormat.setSampleSize(smpl_size);
}

void AudioOutput::SetVolum(float vol)
{
    audioOut->setVolume(vol);
}

