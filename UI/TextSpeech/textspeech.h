#ifndef TEXTSPEECH_H
#define TEXTSPEECH_H

#include <QObject>
#ifdef WIN32
#include<QAxObject>
#endif

class TextSpeech : public QObject
{
    Q_OBJECT
public:
    explicit TextSpeech(QObject *parent = 0);
    ~TextSpeech();
    bool initSpeech();
    bool speak(QString txt);

    void setRate(int rate);

    void setVolume(int value);
    bool isSpeaking();

private:
#ifdef WIN32
    QAxObject _voice;
#endif
    bool _binit;
    bool _bReading;

signals:
    void speakComplete();

private slots:
    void dealevent(QString name, int arc , void* argv);

};

#endif // TEXTSPEECH_H
