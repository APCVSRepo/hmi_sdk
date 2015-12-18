#ifndef TEXTTOSPEECH_H
#define TEXTTOSPEECH_H

#ifdef TTS_ANDROID_SELF
#include "tts.h"
#define DEFAULT_TTS_BUFFERSIZE  1024
struct SynthRequestData {
    int8_t buf[DEFAULT_TTS_BUFFERSIZE];
    int  buf_size;
};
#elif TTS_ESPEAK
#include "speak_lib.h"
#endif
#include <stdint.h>

#include "AudioTrans/AudioOutput.h"

typedef enum
{
    TTS_MODE_SPEAK,
    TTS_MODE_DATA
}TTS_MODE;
typedef void (*ttsSpeak)(const char *data,int len);
class TextToSpeech
{
public:
    TextToSpeech(TTS_MODE mode,int smpl_rate=16000,float rvol=0.5);
    ~TextToSpeech();
    bool TextToVoice(char* src_text);
    //static void* textToVoiceThread(void *date);
    //smpl-rate 采样率：8000,16000，rvol 声音大小,相对比例
   void SetTTSParams(uint32_t &smpl_rate,uint32_t &smpl_size,int &chn,float &rvol);
   void RegisterSpeakFunc(ttsSpeak speak);
   bool ContainsChinese(const char *text,int len);
#ifdef TTS_ANDROID_SELF
   int ttsformat_to_audioformat(android_tts_audio_format_t format);
   android_tts_audio_format_t audioformat_to_ttsformat(int smpl_size);
#endif
   void handleVoiceData(const void *data,int len,int smpl_rate=16000,int smpl_size=16,int chn=1);
private:
    bool checkOpFuncs();
#ifdef TTS_ANDROID_SELF
    static android_tts_callback_status_t TTS_Synth_CB
    (void **pUserData,
     uint32_t trackSamplingHz,
     android_tts_audio_format_t audioFormat,
     int channelCount,
     int8_t **pAudioBuffer,
     size_t *pBufferSize,
     android_tts_synth_status_t status);
#elif TTS_ESPEAK
    static int TTS_Synth_CB(short* data, int len, espeak_EVENT* e);
#endif
private:
    TTS_MODE    m_ttsMode;
    AudioOutput *m_AudioOut;
    ttsSpeak m_ttsFunc_speak;

#ifdef TTS_ANDROID_SELF
    android_tts_engine_t *m_tts_engine;
#elif TTS_ESPEAK
#elif TTS_FLY_MSC
    char m_tts_param[250];
#endif
};

#endif // TEXTTOSPEECH_H
