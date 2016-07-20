#include "JniNative.h"
#include <QMutex>

#if defined(ANDROID)
//javah -jni java.util.Date
//javap -s -p java.util.Date
uchar *JniNative::gBuffer = NULL;
void JniNative::notifyMsg(JNIEnv *env, jobject thiz, int msgNo, int x, int y)
{
    JniFrame::MsgNofityFromJni(msgNo, x, y);
}

void JniNative::setDirectBuffer(JNIEnv *env, jobject thiz, jobject buffer, jint len)
{
    gBuffer = (unsigned char *)env->GetDirectBufferAddress(buffer);
    if (gBuffer == NULL) {
        QDBG << "GetDirectBufferAddress Failed!";
        return;
    }
}

JniNative::JniNative()
{

}

JniNative::~JniNative()
{

}

bool JniNative::registerNativeMethods()
{
    JNINativeMethod methods[] {
        {"notifyMsg", "(III)V", (void*)notifyMsg},
        {"setDirectBuffer","(Ljava/lang/Object;I)V",(void*)setDirectBuffer}
    };

    const char *classname = "an/qt/useJar/ExtendsQtNative";
    jclass clazz;
    QAndroidJniEnvironment env;

    QAndroidJniObject javaClass(classname);
    clazz = env->GetObjectClass(javaClass.object<jobject>());
    QDBG << "find ExtendsQtNative - " << clazz;
    bool result = false;
    if (clazz) {
        jint ret = env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0]));
        env->DeleteLocalRef(clazz);
        QDBG << "RegisterNatives return - " << ret;
        result = ret >= 0;
    }
    if (env->ExceptionCheck())
        env->ExceptionClear();
    return result;
}
#endif
