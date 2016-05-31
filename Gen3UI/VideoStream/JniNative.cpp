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
    //无需拷贝，直接获取与Java端共享的直接内存地址(效率比较高，但object的构造析构开销大，建议长期使用的大型buffer采用这种方式)
    gBuffer = (unsigned char *)env->GetDirectBufferAddress(buffer);
    if( gBuffer == NULL ) {
        QDBG<<"GetDirectBufferAddress Failed!";
        return;
    }

    //可以通过pBuffer指针来访问这段数组的值了,注意，修改数组的值后，Java端同时变化
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
    if(clazz)
    {
        jint ret = env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0]));
        env->DeleteLocalRef(clazz);
        QDBG << "RegisterNatives return - " << ret;
        result = ret >= 0;
    }
    if(env->ExceptionCheck())
        env->ExceptionClear();
    return result;
}
#endif
