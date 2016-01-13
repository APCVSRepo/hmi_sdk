#ifndef APPLINK_H
#define APPLINK_H

#include "AppInclude.h"
#include "Common/AppBase.h"
#include "Common/ScrollBar.h"
#include "Common/AppItemWidget.h"
#include "AppData/AppListInterface.h"
#include "Common/AppListWidget.h"

typedef struct appListStruct
{
    QString appName;
    int  appID;
}AppLinkItem;

typedef enum
{
    APP_ID_FINDAPP=0,
    APP_ID_APPSETTING,
    APP_ID_MORE
}APP_ID_MENU;

class AppLink : public AppBase
{
    Q_OBJECT

public:
    AppLink(AppListInterface * pList, QWidget *parent = 0);
    ~AppLink();

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
    void addNewApp(QString appName,int appID);
    void delNewApp(QString appName);
    void clearNewApp();

private:

//    QWidget m_scrollWidget;

    AppListWidget *m_listWidget;

    QVector<AppLinkItem>  m_appItems;

    void initLayout();
    void flushListWidget();
};

#endif // APPLINK_H
