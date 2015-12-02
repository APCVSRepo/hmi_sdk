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
    return false;
//    if(_binit)
//        return true;
//    audioFormat.setSampleRate(44100);
//    audioFormat.setChannelCount(1);
//    audioFormat.setCodec("audio/pcm");
//    audioFormat.setSampleType(QAudioFormat::SignedInt);
//    audioFormat.setSampleSize(8);
//    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
//    deviceInfo=QAudioDeviceInfo::defaultOutputDevice();
//    if(deviceInfo.isNull()){
//        LOGE("there is no availiable device");
//        return false;
//    }
//    LOGI("default audio output device:%s",deviceInfo.deviceName().toUtf8().data());
//    if(!deviceInfo.isFormatSupported(audioFormat)){
//        audioFormat=deviceInfo.nearestFormat(audioFormat);
//    }
//    LOGI("audio output format: smpl rate=%d,chn=%d,codec=%s",
//         audioFormat.sampleRate(),audioFormat.channelCount(),
//         audioFormat.codec().toUtf8().data());
//    audioOut=new QAudioOutput(deviceInfo,audioFormat);
//    audioDevice=audioOut->start();
//    if(audioDevice==NULL)
//        return false;
//    _binit=true;
//    return true;
}

//文本转语音朗读函数
bool TextSpeech::speak(QString txt)
{
    LOGI("speak:%s",txt.toUtf8().data());
    return false;
//    if(audioDevice==NULL)
//        return false;
//    _bReading = true;
//    audioDevice->write(txt.toUtf8());
//     return true;
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
//    if(name == "EndStream(int,QVariant)")
//    {
//        _bReading = false;
//        emit speakComplete();
//    }
}
#endif
