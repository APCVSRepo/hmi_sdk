#ifndef APPMANAGER_H
#define APPMANAGER_H
#include <QObject>
#include "AppData/AppList.h"
#include "UIInterface.h"
//#include "UI/UIManager.h"
//#include "Gen3UI/Gen3UIManager.h"
#include <QLibrary>

class HMI_Initial:public QObject
{
    Q_OBJECT
public:
    HMI_Initial();
    ~HMI_Initial();

    void ShowUI();

public slots:
    void initApps();
signals:
    void finishMainHMI();

private:
#ifdef SDL_SUPPORT_LIB
    static void initSDL();
    static void* SDLStartThread(void *arg);
#ifdef ANDROID
    static bool FileCopyToConfigdir(const char *dir_);
#endif
#endif
    std::string GetUILibPath();
private:
    AppList *m_appList;
    //CUIManager *m_uiManager;
    //CGen3UIManager *m_uiManager;
    UIInterface *m_uiManager;
    QLibrary m_UILib;
};

#endif // APPMANAGER_H
