#include "JniFrame.h"

AppListInterface *JniFrame::m_pList = NULL;

JniFrame::JniFrame(QObject *parent) : QObject(parent)
{

}

JniFrame::~JniFrame()
{

}

void JniFrame::MsgNofityFromJni(int msgNo, int jniX, int jniY)
{
    int x = jniX * 800.0/1024;
    int y = jniY * 480.0/600;
    switch(msgNo)
    {
    case MSG_NO_ZOOM_IN:
        if(m_pList != NULL)
            if(m_pList->getActiveApp() != NULL)
                m_pList->getActiveApp()->OnSoftButtonClick(ZOOMINBTNID,BUTTON_SHORT);
        break;
    case MSG_NO_ZOOM_OUT:
        if(m_pList != NULL)
            if(m_pList->getActiveApp() != NULL)
                m_pList->getActiveApp()->OnSoftButtonClick(ZOOMOUTBTNID,BUTTON_SHORT);
        break;
    case MSG_NO_MENU:
        if(m_pList != NULL)
            if(m_pList->getActiveApp() != NULL)
            {
                const char appName[13] = {0xE9, 0xAB, 0x98,
                                          0xE5, 0xBE, 0xB7,
                                          0xE5, 0xAF, 0xBC,
                                          0xE8, 0x88, 0xAA, 0x00};
                const char *name = m_pList->getActiveApp()->getAppName().c_str();
                if(0 == strcmp(name, appName))
                {
                }
                else
                {
#ifdef ANDROID
                    QAndroidJniObject::callStaticMethod<void>(
                                "an/qt/useJar/ExtendsQtSurface",
                                "stop","()V");
#endif
                    m_pList->getActiveApp()->OnShowCommand();
                }

            }
        break;
    case MSG_NO_PRESS:
        if(m_pList != NULL)
            if(m_pList->getActiveApp() != NULL)
                m_pList->getActiveApp()->OnVideoScreenTouch(TOUCH_START,x,y);
        break;
    case MSG_NO_RELEASE:
        if(m_pList != NULL)
            if(m_pList->getActiveApp() != NULL)
                m_pList->getActiveApp()->OnVideoScreenTouch(TOUCH_END,x,y);
        break;
    case MSG_NO_MOVE:
        if(m_pList != NULL)
            if(m_pList->getActiveApp() != NULL)
                m_pList->getActiveApp()->OnVideoScreenTouch(TOUCH_MOVE,x,y);
        break;
    case MSG_NO_CMD_1:
        m_pList->OnAppExit();
#ifdef ANDROID
        QAndroidJniObject::callStaticMethod<void>(
                    "an/qt/useJar/ExtendsQtSurface",
                    "stop","()V");
#endif
        break;
    }
}

void JniFrame::setAppList(AppListInterface *pList)
{
    m_pList = pList;
}

void JniFrame::stopStream()
{
#ifdef ANDROID
    QAndroidJniObject::callStaticMethod<void>(
                "an/qt/useJar/ExtendsQtSurface",
                "stop","()V");
#endif
}
