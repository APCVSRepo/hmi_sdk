#ifndef JNINATIVE_H
#define JNINATIVE_H

#include <QApplication>
#if defined(ANDROID)
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <jni.h>
#endif
#include "JniFrame.h"
#include <QDebug>

#define QDBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__

#define MAX_BUF_SIZE 1024*1024*2

class JniNative
{
public:
    JniNative();
    ~JniNative();
    bool registerNativeMethods();
#ifdef ANDROID
    static void notifyMsg(JNIEnv *env, jobject thiz, int msgNo, int x, int y);
#endif

    static uchar *gBuffer;
#ifdef ANDROID
    static void setDirectBuffer(JNIEnv *env, jobject thiz, jobject buffer, jint len);
#endif
private:

};

#endif // JNINATIVE_H
