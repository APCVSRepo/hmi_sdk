#ifndef APPLIST_UI_H
#define APPLIST_UI_H

#include "UIInclude.h"
#include "Common/AppBase.h"
#include "Common/ScrollBar.h"
#include "Common/AppItemWidget.h"
#include "AppData/AppListInterface.h"
#include "Common/AppListWidget.h"

typedef struct appListStruct
{
    QString appName;
    int  appID;
    QString strIconPath;

    appListStruct():appName(""),appID(0),strIconPath("")
    {

    }
}AppListItem;

typedef enum
{
    APP_ID_FINDAPP=0,
    APP_ID_APPSETTING,
    APP_ID_MORE
}APP_ID_MENU;

class AppListUI : public AppBase
{
    Q_OBJECT

public:
    AppListUI(AppListInterface * pList, QWidget *parent = 0);
    ~AppListUI();

protected:
    virtual void showEvent(QShowEvent * e);

signals:
    void inAppSignals(int appID);
    void appSetting();
    void findNewApp();

private slots:

    void listWidgetDoubleClickedSlots(QModelIndex);
    void inAppSlots(int appID);
    void findNewAppSlots();

private:
    void addNewApp(QString appName,int appID,QString appIconPath);
    void addNewApp(QString appName,int appID);
    void delNewApp(QString appName);
    void clearNewApp();

private:

//    QWidget m_scrollWidget;

    AppListWidget *m_listWidget;

    QVector<AppListItem>  m_appItems;

    void initLayout();
/*
    void addListItemWidget(QString text);
    void addListItemWidget(QString text,QString strPath);
    void flushAllItems(int currentNo);
*/
    void flushListWidget();
};

#endif // APPLIST_UI_H
