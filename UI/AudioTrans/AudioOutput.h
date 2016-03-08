#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H
#include <QTMultiMedia/QAudioOutput>
#include <QIODevice>
#include <QMutex>
#include <QTimerEvent>

class AudioOutput:public QObject
{
    Q_OBJECT
public:
    AudioOutput(int smpl_rate=16000,int smpl_size=16,QObject *parent=NULL);
    ~AudioOutput();
    bool Speak(char *data,int len);
    void SetVolum(float vol);
    void SetSmplRate(int smpl_rate);
    void SetSmplSmplSize(int smpl_size);
    void OpenAudio();
    void CloseAudio();

private:
    //void timerEvent(QTimerEvent *e);
    bool restartAudioOutDevice();
    void getByteArray(char *data,int &len);
private:
    QAudioDeviceInfo deviceInfo;
    QIODevice   *audioDevice;
    QAudioFormat audioFormat;
    QAudioOutput *audioOut;
    QMutex        audioOutMutex;
   // QByteArray   audioBuffer;
};

#endif // AUDIOOUTPUT_H
