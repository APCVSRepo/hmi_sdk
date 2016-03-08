#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include "Common/MenuButton.h"
#include <QTimer>
#include <QTextEdit>
#include <AppData/AppListInterface.h>
#include "VideoStream/VideoStream.h"


class MainWindow : public QWidget
{
    typedef enum
    {
        MENU_MUSIC=0,
        MENU_MF,
        MENU_PTHONE,
        MENU_HOME,
        MENU_APPLIST,
        MENU_SETTING,
        MENU_MAX
    }MENU_APP_TYPE;

    Q_OBJECT
public:
    explicit MainWindow(AppListInterface * pList,QWidget *parent = 0);
    ~MainWindow();
    QWidget* CenterWidget();
    //void InsertChildApp(int index,int funcId,QString text,QString on,QString off);
    //void DeleteChildApp(int index);
//    void ExeRPC();

    void StartVideoStream(const char* url);
    void StopVideoStream();

    bool InVideoStream();

    void BackToVideoStream();
protected:
    virtual void showEvent(QShowEvent * e);

signals:

public slots:
    void onUpdateTime();
    void onMenuSelected();
    //void onChildAppSelected(int funcId);

    void OnVideoStreamMenuBtnClicked();
private:
    VideoStream* videoWidget;

    QLabel *m_pIcon;
    QLabel *m_pTime;
    QLabel *m_pNetStatus;
    QWidget *m_pCenter;
    QWidget  *m_pMainMenu;
    MenuButton  *m_pMenuTab[MENU_MAX];
    QTimer  *m_pTimer;
    //QList<CAppButton*> m_pChildApps;

    AppListInterface *m_pList;
    int  m_AppWidth;
    int  m_AppHeight;

    bool m_bInVideoStream;
};

#endif // APPLINKMENU_H
