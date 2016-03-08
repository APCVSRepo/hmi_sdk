#ifndef MSP_VR_AUDIO_H
#define MSP_VR_AUDIO_H

#include <QUrl>
#include <QAudioRecorder>
#include<QTimer>
#include<QAudioBuffer>
#include <QAudioProbe>
#include<qaudioinput.h>
#include"AudioInput.h"

#include <QSharedMemory>
typedef short int16_t;
typedef unsigned int uint32_t;

class msp_vr_audio:public QObject
{
    Q_OBJECT
public:
    msp_vr_audio();
    ~msp_vr_audio();
    void record_start();
    void record_end();
    void  audioFormat();
    QAudioFormat buildFormat(int smpl_rate,int bits_per_smpl,int audio_type);
    static void* loopRecordThread(void *arg);
public slots:
    void loopRecord();
    void handleProbBuffer(bool isOver,char *buffer,int len);
private:
    AudioInput      *audioDevice;
    int            counter;

    quint32 m_maxAmplitude;
    QTimer    *timer;

    int vr_type;
    int max_duration;
    int sampling_rate;
    int bits_per_sample;
    int audio_type;
};

#endif // MSP_VR_AUDIO_H
