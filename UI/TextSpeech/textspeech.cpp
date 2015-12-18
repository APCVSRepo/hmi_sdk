#include "textspeech.h"
#include "Config/Config.h"



TextSpeech::TextSpeech(QObject *parent) :
    QObject(parent),_binit(0),_bReading(0)
{
    initSpeech();//初始化语音
}

TextSpeech::~TextSpeech()
{
}

#ifdef WIN32
//初始化语音函数
bool TextSpeech::initSpeech()
{
    if(_binit)
        return true;

    _binit = this->_voice.setControl("96749377-3391-11D2-9EE3-00C04F797396");//设置COM的名称用来初始化COM的模型,返回是否加载成功

    if(_binit)
    {
        connect(&this->_voice,SIGNAL(signal(QString, int, void*)), this, SLOT(dealevent(QString, int, void*)));
    }

    return _binit;
}

//文本转语音朗读函数
bool TextSpeech::speak(QString txt)
{
    if(!_binit)
        return false;
    int result = this->_voice.dynamicCall("Speak(QString, SpeechVoiceSpeakFlags)", txt ,1).toInt();//执行第一个参数的方法名，以第二个参数返回是否执行成功
    _bReading = true;
    return result;//返回是否执行阅读函数
}

//判断语音系统是否运行函数
bool TextSpeech::isSpeaking()
{
    return _bReading;
}

//设置语音朗读速度-10到10
void TextSpeech::setRate(int rate)
{
    if(!_binit)
        return;

    this->_voice.dynamicCall("SetRate(int)", rate);
}

//设置语音音量0到100
void TextSpeech::setVolume(int value)
{
    if(!_binit)
        return;

    this->_voice.dynamicCall("SetVolume(int)", value);
}

void TextSpeech::dealevent(QString name, int arc , void* argv)
{
    if(name == "EndStream(int,QVariant)")
    {
        _bReading = false;
        emit speakComplete();
    }
}
#else
//初始化语音函数
bool TextSpeech::initSpeech()
{
    if(_binit)
        return true;
    _voice=new TextToSpeech(TTS_MODE_SPEAK);
    _binit=true;
    return true;
}

//文本转语音朗读函数
bool TextSpeech::speak(QString txt)
{
    LOGI("speak:voice");
    if(_voice==NULL)
        return false;
    _bReading = true;
    bool ret= _voice->TextToVoice(txt.toUtf8().data());
    _bReading = false;
    return ret;
}

//判断语音系统是否运行函数
bool TextSpeech::isSpeaking()
{
    return _bReading;
}


//设置语音朗读速度-10到10
void TextSpeech::setRate(int rate)
{
//    audioFormat.setSampleRate(rate);
}

//设置语音音量0到100
void TextSpeech::setVolume(int value)
{
//    if(audioOut==NULL)
//        return;
//    audioOut->setVolume(value);
}

void TextSpeech::dealevent(QString name, int arc , void* argv)
{
    if(name == "EndStream(int,QVariant)")
    {
        _bReading = false;
        emit speakComplete();
    }
}
#endif
