#include "AudioInput.h"

#include <stdlib.h>
#include <math.h>

#include <QDateTime>
#include <QAudioInput>
#include <qendian.h>
#include"lib_msp_vr.h"
#define PUSH_MODE_LABEL "Enable push mode"
#define PULL_MODE_LABEL "Enable pull mode"
#define SUSPEND_LABEL   "Suspend recording"
#define RESUME_LABEL    "Resume recording"
#include "Config/Config.h"

const int BufferSize = 4096;

PCM_HANDLE   pcmHandle;

AudioInput::AudioInput(int smpl_rate,int bits_per_smpl,int audio_type, QObject *parent)
    :   QObject(parent)
    ,   m_maxAmplitude(0)
    ,   m_level(0.0)

{
    setFormat(smpl_rate,bits_per_smpl,audio_type);
}

QAudioFormat AudioInput::buildFormat(int smpl_rate,int bits_per_smpl,int audio_type)
{
    QAudioFormat format;
    if(smpl_rate==16000)
        format.setSampleRate(22050);//8kHZ
    else
      format.setSampleRate(smpl_rate);
    format.setChannelCount(1);
    format.setSampleSize(bits_per_smpl);//default 8
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format)) {
        LOGI("Default format not supported - trying to use nearest");
        QAudioFormat format_n = info.nearestFormat(format);
        return format_n;
    }
    return format;
}

void AudioInput::sendFormatData(const char *buffer, QAudioFormat &format, int len)
{
    qreal level=0;
    char  *pcm_data=(char*)buffer;
    int   pcm_len=len;
    LOGI("buffer len=%d,format type=%d\n",len,format.sampleType());

    int in_rate=format.sampleRate();
    int in_size=format.sampleSize();
    if(m_set_sample_rate!=in_rate)
    {
        char sample_buffer[5000]={0};
        int sample_len=0;
        LOGI("sample rate=%d,size=%d\n",in_rate,in_size);
#ifdef SDL_SUPPORT_VR
        int error=soxr_wav_ratio_convert(in_rate,m_set_sample_rate,in_size,(char*)buffer,len,sample_buffer,&sample_len);
        LOGI("error=%d,sample convert: buffer len=%u\n",error,sample_len);
        if(error==0)
        {
            pcm_data=sample_buffer;
            pcm_len=sample_len;
        }
        else
#endif
            return;

    }
    else
    {
        level=audioLevel(pcm_data,pcm_len);
        LOGI("level=%f\n",level);
        if(level<0.01)
        {
            isOver++;
        }
        else
        {
            isOver=0;
        }
    }
    emit pushBuffer(isOver>5,pcm_data,pcm_len);
}

void AudioInput::setFormat(int smpl_rate,int bits_per_smpl,int audio_type)
{
    m_format=buildFormat(smpl_rate,bits_per_smpl,audio_type);
    audioInput=new QAudioInput(QAudioDeviceInfo::defaultInputDevice(),m_format,this);
    m_set_sample_rate=smpl_rate;
    switch (m_format.sampleSize()) {
    case 8:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 32767;
            break;
        default:
            break;
        }
        break;

    case 32:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 0xffffffff;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 0x7fffffff;
            break;
        case QAudioFormat::Float:
            m_maxAmplitude = 0x7fffffff; // Kind of
        default:
            break;
        }
        break;

    default:
        break;
    }
}

AudioInput::~AudioInput()
{
}

void AudioInput::start()
{
    device=audioInput->start();
    QAudioFormat format=m_format;
#ifdef SDL_SUPPORT_VR
    if(m_set_sample_rate!=format.sampleRate())
        soxr_wav_convert_init(format.sampleRate(),m_set_sample_rate,format.sampleSize());
#endif
    LOGI("byteOrder=%d,frameByte=%d,chnnel=%d,sampleRate=%d,sampleSize=%d,sampleType=%d\n",format.byteOrder(),format.bytesPerFrame(),format.channelCount(),format.sampleRate(),
           format.sampleSize(),format.sampleType());
    if(!isOpen())
    {
        LOGI("audio device is not open");
        return;
    }
    isOver=0;
#if  (RECORD_HAS_FILE)
#ifdef SDL_SUPPORT_VR
    pcmHandle=soxr_pcm_open("inputAudio.wav");
#endif
    m_total_size=0;
#endif
    //disconnect(device,SIGNAL(readyRead()),SLOT(onReadyRead()));
    connect(device,SIGNAL(readyRead()),SLOT(onReadyRead()));
}

void AudioInput::stop()
{
    audioInput->stop();
#if  (RECORD_HAS_FILE)
#ifdef SDL_SUPPORT_VR
    soxr_pcm_close(pcmHandle,m_total_size,m_format.sampleRate(),m_format.sampleSize());
#endif
#endif
}
 bool  AudioInput::isOpen()
 {
     if(device==NULL)
         return false;
     return device->isOpen();
 }

 QAudio::State AudioInput::state()
 {
     if(audioInput==NULL)
         return QAudio::StoppedState;
     return audioInput->state();
 }
 void AudioInput::onReadyRead()
 {
     qint64 len = audioInput->bytesReady();
     char buffer[5000];

     qint64 l =device->read(buffer, len);

     if (l > 0)
     {
         writeData(buffer,l);
         sendFormatData(buffer,m_format,l);
     }
 }
qint64 AudioInput::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    Q_UNUSED(maxlen)

    return 0;
}

qreal  AudioInput::audioLevel(const char *data, qint64 len)
{
    if (m_maxAmplitude) {
        Q_ASSERT(m_format.sampleSize() % 8 == 0);
        const int channelBytes = m_format.sampleSize() / 8;
        const int sampleBytes = m_format.channelCount() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const int numSamples = len / sampleBytes;

        quint32 maxValue = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

        for (int i = 0; i < numSamples; ++i) {
            for (int j = 0; j < m_format.channelCount(); ++j) {
                quint32 value = 0;

                if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
                } else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    value = qAbs(*reinterpret_cast<const qint8*>(ptr));
                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint16>(ptr);
                    else
                        value = qFromBigEndian<quint16>(ptr);
                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint16>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint16>(ptr));
                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint32>(ptr);
                    else
                        value = qFromBigEndian<quint32>(ptr);
                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint32>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint32>(ptr));
                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::Float) {
                    value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff); // assumes 0-1.0
                }

                maxValue = qMax(value, maxValue);
                ptr += channelBytes;
            }
        }

        maxValue = qMin(maxValue, m_maxAmplitude);
        m_level = qreal(maxValue) / m_maxAmplitude;
    }
    else
        return 0;
    return m_level;
}

qint64 AudioInput::writeData(const char *data, qint64 len)
{
#if  (RECORD_HAS_FILE)
    m_total_size+=len;
    LOGI("write len=%d\n",m_total_size);
#ifdef SDL_SUPPORT_VR
    soxr_pcm_write(pcmHandle,(char*)data,len);
#endif
#endif
    return len;
}

