#ifndef APPLINK_H
#define APPLINK_H

#include "AppInclude.h"
#include "Common/AppBase.h"
#include "Common/ScrollBar.h"
#include "Common/AppItemWidget.h"
#include "AppData/AppListInterface.h"

typedef struct appListStruct
{
    QString str_appName;
    int i_appID;
}SAppList;

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

    void listWidgetClickedSlots(QModelIndex);
    void listWidgetDoubleClickedSlots(QModelIndex);

    void onScrollTo(int v);
//    void upArrowSlots();
//    void downArrowSlots();

    void inAppSlots(int appID);
    void findNewAppSlots();

private:
    void addNewApp(QString appName,int appID);
    void delNewApp(QString appName);
    void clearNewApp();

private:

//    QWidget m_scrollWidget;
    ScrollBar m_scrollWidget;
    QListWidget m_listWidget;

    QVector <QListWidgetItem *> m_vec_listItem;
    QVector <AppItemWidget *> m_vec_appItemWidget;
    QVector <QString > m_vec_string;
//    QMap <QString, QString> m_map_appNameID;
    QVector <SAppList > m_vec_appList;

    int m_i_currentRow;
    int m_i_totalRow;

    int m_i_showRow;
    bool m_b_downUp;

    void initLayout();
    void addListItemWidget(QString text);
    void flushAllItems(int currentNo);
    void flushListWidget();
};

#endif // APPLINK_H
