#ifndef TEXTSPEECH_H
#define TEXTSPEECH_H

#include <QObject>
#ifdef WIN32
#include<QAxObject>
#endif


#include "TextToSpeech.h"

class TextSpeech : public QObject
{
enum{TTS_STAT_START = 0,TTS_STAT_COMPLETE,TTS_STAT_INTERRUPT};

    Q_OBJECT
public:
    explicit TextSpeech(QObject *parent = 0);
    ~TextSpeech();
    bool initSpeech();
    bool speak(QString txt);

    void setRate(int rate);

    void setVolume(int value);
    bool isSpeaking();

    bool StartVoiceThread(std::string string);
private:
#ifdef WIN32
    QAxObject _voice;
    static unsigned int __stdcall ThreadTextToSpeech(void *arg);
    quint64 m_hThread;
#else
    TextToSpeech *_voice;
#endif
    bool _binit;
    bool _bReading;

    std::string m_strToSpeak;
    void SpeakThreadComplete();
signals:
    void speakComplete();
private slots:    
    void dealevent(QString name, int arc , void* argv);
    void MyExceptionSlot(int,QString,QString,QString);
};

#endif // TEXTSPEECH_H
