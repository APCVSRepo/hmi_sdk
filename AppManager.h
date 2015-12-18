#ifndef APPMANAGER_H
#define APPMANAGER_H
#include <QObject>
#include "AppData/AppList.h"
#include "UI/UIManager.h"

class AppManager:public QObject
{
    Q_OBJECT
public:
    AppManager();
    ~AppManager();

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

private:
    AppList *m_appList;
    CUIManager *m_uiManager;

};

#endif // APPMANAGER_H
