#include "TextToSpeech.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#ifdef TTS_ANDROID_SELF
#include "tts.h"
//#include "TtsEngine.h"
#elif TTS_ESPEAK
#include "speak_lib.h"
#elif TTS_FLY_MSC
#include <msp_types.h>
#include <qtts.h>
#include <msp_cmn.h>
#include <msp_errors.h>
#endif
#include <Config/Config.h>



TextToSpeech::TextToSpeech(TTS_MODE mode,int smpl_rate,float rvol)
{
#ifdef  TTS_ANDROID_SELF
    m_tts_engine=getTtsEngine();
    if(NULL == m_tts_engine){
        LOGE("TTS engine not availiable");
        return;
    }
    android_tts_result_t  result=
            m_tts_engine->funcs->init(m_tts_engine,TTS_Synth_CB,"com.svox.pico");//"com.google.tts"
    android_tts_support_result_t lan_result=
            m_tts_engine->funcs->isLanguageAvailable(m_tts_engine,"zh",NULL,NULL);
    LOGI("init result %d,chinese availiable %d",result,lan_result);

    m_tts_engine->funcs->setLanguage(m_tts_engine,"eng","USA","");//"zh","CN"

    char bitch[20]={};
    char rate[20]={};
    size_t size,size2;

    m_tts_engine->funcs->getProperty(m_tts_engine,
                                     ANDROID_TTS_ENGINE_PROPERTY_PITCH,
                                     bitch,&size);
    m_tts_engine->funcs->getProperty(m_tts_engine,
                                     ANDROID_TTS_ENGINE_PROPERTY_RATE,
                                     rate,&size2);
    LOGI("bitch:%s,%d,rate:%s,%d",bitch,size,rate,size2);
   // smpl_rate=pRate;
#elif TTS_ESPEAK

    int rate=espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS,0,"/sdcard",0);
    if(rate==-1){
        LOGE("ESPEAK  initialize failed");
    }else{
        smpl_rate=rate;
    }
    LOGI("espeak callback set");
    espeak_SetSynthCallback(TextToSpeech::TTS_Synth_CB);   // 设置回调函数
            LOGI("espeak voice");
    espeak_ERROR erro=espeak_SetVoiceByName("zh+f2");
     LOGI("espeak voice erro=%d",erro);
     espeak_VOICE *voice=espeak_GetCurrentVoice();
     LOGI("name=%s,languages=%s,iden=%s,gender=%d,age=%d",
          voice->name,voice->languages,voice->identifier,voice->gender,voice->age);
     voice->age=23;
     voice->gender=2;
     espeak_SetVoiceByProperties(voice);
#endif
    uint32_t smpl_size=16;
    int chn=1;
    uint32_t smpl_rate0=(int)smpl_rate;
    SetTTSParams(smpl_rate0,smpl_size,chn,rvol);
    m_ttsMode=mode;
    if(mode==TTS_MODE_SPEAK)
    {
        m_AudioOut=new AudioOutput(smpl_rate,smpl_size);
    }

}

TextToSpeech::~TextToSpeech()
{
#ifdef  TTS_ANDROID_SELF
    if(m_tts_engine)
        m_tts_engine->funcs->shutdown(m_tts_engine);
#elif TTS_ESPEAK
    espeak_Terminate();
#elif TTS_FLY_MSC
    MSPLogout();
#endif
    if(m_ttsMode==TTS_MODE_SPEAK)
        delete m_AudioOut;
}

void TextToSpeech::RegisterSpeakFunc(ttsSpeak speak)
{
    m_ttsFunc_speak=speak;
}

void TextToSpeech::SetTTSParams(uint32_t &smpl_rate,uint32_t &smpl_size,int &chn,float &rvol)
{
#ifdef  TTS_ANDROID_SELF
    android_tts_audio_format_t pEncoding=
            audioformat_to_ttsformat(smpl_size);

    m_tts_engine->funcs->setAudioFormat(m_tts_engine,&pEncoding,&smpl_rate,&chn);
    smpl_size = ttsformat_to_audioformat(pEncoding);

//    m_tts_engine->funcs->setProperty(m_tts_engine,
//                                     ANDROID_TTS_ENGINE_PROPERTY_RATE,
//                                     "90",strlen("90"));
//    char vol[10]={0};
//    sprintf(vol,"%d",(int)(rvol*500));
//    m_tts_engine->funcs->setProperty(m_tts_engine,
//                                     ANDROID_TTS_ENGINE_PROPERTY_VOLUME,
//
//    vol,strlen(vol));
#elif TTS_ESPEAK

#elif TTS_FLY_MSC
    sprintf(m_tts_param,"engine_type = local, text_encoding = UTF8, tts_res_path = fo|%s/xiaoyan.jet;fo|%s/common.jet, sample_rate = %d",CONFIG_DIR,CONFIG_DIR,smpl_rate);
    //sprintf(m_tts_param,"vcn=xiaoyu ,ttp= text ,tte= UTF8,aue= speex ,auf = audio/L16;rate= %d,spd= default ,vol=%d",smpl_rate,(int)(rvol*10));
    LOGI(m_tts_param);
#endif
}

bool TextToSpeech::checkOpFuncs()
{
    if(m_ttsMode==TTS_MODE_SPEAK){
        if(m_AudioOut==NULL){
            LOGE("m_AudioOut is null");
            return false;
        }
    }
    else{
        if(NULL == m_ttsFunc_speak){
            LOGE("m_ttsFunc_speak is null");
            return false;
        }
    }
    return true;
}

bool TextToSpeech::ContainsChinese(const char *text, int len)
{
    int i = 0;
    while (i < len){
        unsigned char dw = (unsigned char)text[i];
        if (dw >= 0xa1){
            if (dw != 0xa3){//chinese
                return true;
            }
            i = i + 2;
        }
        else{//english
            i = i + 1;

        }
    }
    return false;
}

void TextToSpeech::handleVoiceData(const void *data, int len,int smpl_rate,int smpl_size,int chn)
{

    if(m_ttsMode==TTS_MODE_SPEAK){
        //m_AudioOut->OpenAudio(smpl_rate,smpl_size,chn);
        //m_AudioOut->SetSmplRate(smpl_rate);
        //m_AudioOut->SetSmplSmplSize(smpl_size);

        m_AudioOut->Speak((char *)data,len);
        //m_AudioOut->CloseAudio();
    }
    else{
        m_ttsFunc_speak((const char*)data,len);
    }
}


bool TextToSpeech::TextToVoice(char* src_text)
{
    if(m_ttsMode==TTS_MODE_SPEAK){
        m_AudioOut->OpenAudio();
    }
#ifdef  TTS_ANDROID_SELF
    if(NULL == m_tts_engine){
        LOGE("tts engine is null");
        return false;
    }
     SynthRequestData  *synthData = new SynthRequestData ;
     synthData->buf_size=DEFAULT_TTS_BUFFERSIZE;
     memset(synthData->buf,0,DEFAULT_TTS_BUFFERSIZE);
     android_tts_result_t result=m_tts_engine->funcs->synthesizeText(
                 m_tts_engine,src_text,
                 synthData->buf,
                 synthData->buf_size,
                 this);
     LOGI("TextToVoice status=%d",result);
     delete synthData;
     return result==ANDROID_TTS_SUCCESS;
#elif TTS_ESPEAK
    if(ContainsChinese((const char *)src_text,strlen(src_text))){
        espeak_SetVoiceByName("zh+f2");
    }
    else{
        espeak_SetVoiceByName("en-us+f2");
    }
    espeak_Synth((const void *)src_text,strlen(src_text)+1,0,POS_CHARACTER,0,espeakCHARS_UTF8,NULL,this);
    espeak_ERROR erro=espeak_Synchronize();
    if(EE_OK !=erro){
        LOGE("synth erro=%d",erro);
    }
 #elif TTS_FLY_MSC
    const char* sess_id = NULL;
    int ret = -1;
    unsigned int text_len = 0;
    unsigned int audio_len = 0;
    int synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;

    LOGI("begin to synth...\n");
    if (NULL == src_text  )
    {
        LOGE("params is null!:%s\n",src_text);
        return false;
    }
    if(!checkOpFuncs())
        return false;
    const char* login_configs = " appid = 5668ed58,work_dir = /sdcard/sdlconfig/";//登录参数
    ret=MSPLogin("myxuan475@126.com","xf1032075878myx",login_configs);
    if(0 != ret){
        LOGE("MspLogin failed");
    }
    text_len = (unsigned int)strlen(src_text);
    sess_id = QTTSSessionBegin(m_tts_param, &ret);
    if ( ret != MSP_SUCCESS )
    {
        MSPLogout();
        LOGE("QTTSSessionBegin: qtts begin session failed Error code %d.\n",ret);
        return false;
    }

    ret = QTTSTextPut(sess_id, src_text, text_len, NULL );
    if ( ret != MSP_SUCCESS )
    {
        LOGE("QTTSTextPut: qtts put text failed Error code %d.\n",ret);
        QTTSSessionEnd(sess_id, "TextPutError");
        MSPLogout();
        return ret;
    }
    while (1)
    {
        const void *data = QTTSAudioGet(sess_id, &audio_len, &synth_status, &ret);
        if (NULL != data)
        {
            LOGI("voice:len=%d",audio_len);
            handleVoiceData(data,audio_len);
        }
        if (synth_status == MSP_TTS_FLAG_DATA_END || ret != 0)
            break;
    }//合成状态synth_status取值可参考开发文档

    ret = QTTSSessionEnd(sess_id, NULL);
    if ( ret != MSP_SUCCESS )
    {
        LOGE("QTTSSessionEnd: qtts end failed Error code %d.\n",ret);
    }
    MSPLogout();
    return ret==0;
#endif
    if(m_ttsMode==TTS_MODE_SPEAK){
        m_AudioOut->CloseAudio();
    }
    return true;
}




#ifdef TTS_ANDROID_SELF
android_tts_callback_status_t TextToSpeech::TTS_Synth_CB
            (void **pUserData,
             uint32_t trackSamplingHz,
             android_tts_audio_format_t audioFormat,
             int channelCount,
             int8_t **pAudioBuffer,
             size_t *pBufferSize,
             android_tts_synth_status_t status)
{
    LOGI("status:%d,hz:%d,chn:%d,size:%d",status,
         trackSamplingHz,channelCount,*pBufferSize);
    if(*pBufferSize>0){
        TextToSpeech *ttsHandle=static_cast<TextToSpeech*>(*pUserData);
        ttsHandle->handleVoiceData(*pAudioBuffer,*pBufferSize,
                                   trackSamplingHz,ttsHandle->ttsformat_to_audioformat(audioFormat),channelCount);

    }
}

int TextToSpeech::ttsformat_to_audioformat(android_tts_audio_format_t format)
{
    int smpl_size=16;
    switch(format)
    {
    case ANDROID_TTS_AUDIO_FORMAT_DEFAULT:
        smpl_size=16;
        break;
    case ANDROID_TTS_AUDIO_FORMAT_PCM_16_BIT:
        smpl_size=16;
        break;
    case ANDROID_TTS_AUDIO_FORMAT_PCM_8_BIT:
        smpl_size=8;
        break;
    }
    return smpl_size;
}

android_tts_audio_format_t TextToSpeech::audioformat_to_ttsformat(int smpl_size)
{
    android_tts_audio_format_t pEncoding;
    if(smpl_size == 8)
        pEncoding=ANDROID_TTS_AUDIO_FORMAT_PCM_8_BIT;
    else if(smpl_size == 16)
        pEncoding=ANDROID_TTS_AUDIO_FORMAT_PCM_16_BIT;
    else
        pEncoding=ANDROID_TTS_AUDIO_FORMAT_DEFAULT;
    return pEncoding;
}
#elif TTS_ESPEAK
int TextToSpeech::TTS_Synth_CB(short *data, int len, espeak_EVENT *e)
{
    LOGI("synth len=%d",len);
    if(data==NULL)
        return 1;
    if(len>0){
        TextToSpeech *ttsHandle=static_cast<TextToSpeech*>(e->user_data);
        ttsHandle->handleVoiceData((void*)data,len*2
                                  );
    }
    return 0;

}

#endif

