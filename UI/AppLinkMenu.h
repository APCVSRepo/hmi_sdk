#ifndef APPLINKMENU_H
#define APPLINKMENU_H

#include <QWidget>
#include <QLabel>
#include "Common/MenuButton.h"
#include <QTimer>
#include <QTextEdit>
#include "Gen3UI/AppListView/CAppButton.h"
#include <AppData/AppListInterface.h>

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

class AppLinkMenu : public QWidget
{
    Q_OBJECT
public:
    explicit AppLinkMenu(AppListInterface * pList,QWidget *parent = 0);
    ~AppLinkMenu();
    QWidget* CenterWidget();
    void InsertChildApp(int index,int funcId,QString text,QString on,QString off);
    void DeleteChildApp(int index);
//    void ExeRPC();
protected:
    virtual void showEvent(QShowEvent * e);

signals:

public slots:
    void onUpdateTime();
    void onMenuSelected();
    void onChildAppSelected(int funcId);
private:
    QLabel *m_pIcon;
    QLabel *m_pTime;
    QLabel *m_pNetStatus;
    QWidget *m_pCenter;
    QWidget  *m_pMainMenu;
    MenuButton  *m_pMenuTab[MENU_MAX];
    QTimer  *m_pTimer;
    QList<CAppButton*> m_pChildApps;

    AppListInterface *m_pList;
    int  m_AppWidth;
    int  m_AppHeight;
};

#endif // APPLINKMENU_H
