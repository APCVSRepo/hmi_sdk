#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QIODevice>
#include<QAudioFormat>
#include<qobject.h>
#include<QAudioInput>
#ifdef SDL_SUPPORT_VR
#include"lib_msp_vr.h"
#endif
class AudioInput : public QObject
{
    Q_OBJECT

public:
    AudioInput(int smpl_rate,int bits_per_smpl,int audio_type, QObject *parent);
    ~AudioInput();

    QAudioFormat buildFormat(int smpl_rate,int bits_per_smpl,int audio_type);
    void setFormat(int smpl_rate,int bits_per_smpl,int audio_type);
    void sendFormatData(const char *buffer,QAudioFormat&,int len);

    void start();
    void stop();

    bool  isOpen();
    QAudio::State  state();

    qreal level() const { return m_level; }
    qreal audioLevel(const char *data, qint64 len);

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
public slots:
    void onReadyRead();
private:
    QAudioFormat m_format;
    quint32 m_maxAmplitude;
    qreal m_level; // 0.0 <= m_level <= 1.0
    int isOver;

    int  m_set_sample_rate;
#if  (RECORD_HAS_FILE)
    int  m_total_size;
#endif

    QAudioInput    *audioInput;
    QIODevice  *device;
signals:
    void pushBuffer(bool isOver,char *data,int len);


};

#endif // AUDIOINPUT_H
