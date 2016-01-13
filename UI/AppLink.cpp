#include "AppLink.h"
#include "UI/Notify/Notify.h"
#include "Common/AppBase.h"
AppLink::AppLink(AppListInterface * pList, QWidget *parent)
    : AppBase(pList, parent)
{
    m_listWidget = new AppListWidget(ui_app_width*0.1,0,ui_app_width*0.8,ui_app_height,this);
    connect(this,SIGNAL(inAppSignals(int)),this,SLOT(inAppSlots(int)));
    connect(this,SIGNAL(findNewApp()),this,SLOT(findNewAppSlots()));

    connect(m_listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(listWidgetDoubleClickedSlots(QModelIndex)));
    initLayout();
}

AppLink::~AppLink()
{
}

void AppLink::initLayout()
{

}



//双击某行，可进行某一行的子菜单;
void AppLink::listWidgetDoubleClickedSlots(QModelIndex index)
{
    int appID = m_appItems.at(index.row()).appID;

    if(appID > 1)
        emit inAppSignals(appID);
    else if(index.row()+1 == m_listWidget->count())
    {
        //App Setting
        emit appSetting();
    }
    else if(index.row()+2 == m_listWidget->count())
    {
        //Find New App
        emit findNewApp();
    }
}

//有新的APP上线，增加显示到list的最上一行;
void AppLink::addNewApp(QString appName, int appID)
{
//    m_vec_string.insert(0,appTitle);
    AppLinkItem linkItem;
    linkItem.appID=appID;
    linkItem.appName=appName;
    m_appItems.insert(0,linkItem);
}
void AppLink::clearNewApp()
{
    m_appItems.clear();

    AppLinkItem linkItem1,linkItem2;
    linkItem1.appID=APP_ID_APPSETTING;
    linkItem1.appName="App settings";
    linkItem2.appID=APP_ID_FINDAPP;
    linkItem2.appName="Find new Apps";
    m_appItems.append(linkItem1);
    m_appItems.append(linkItem2);
}

//刷新list列表
void AppLink::flushListWidget()
{
    m_listWidget->DelListItemWidget();
    m_listWidget->SetScrollParams(4,m_appItems.count());
    for(int i=0;i<m_appItems.count();i++){
        m_listWidget->AddListItemWidget(m_appItems.at(i).appName,true);
    }

	m_listWidget->FlushAll();
}

void AppLink::inAppSlots(int appID)
{
    m_pList->OnAppActivated(appID);
}



void AppLink::findNewAppSlots()
{
    Notify n(this->parentWidget());
    n.showDlg();
}

void AppLink::showEvent(QShowEvent * e)
{
    // TODO: getData();
    clearNewApp();
    setTitle("Mobile Apps");

    std::vector<int> vAppIDs;
    std::vector<std::string> vAppNames;
    m_pList->getAppList(vAppIDs, vAppNames);
    for (int i = 0; i < vAppIDs.size(); i++)
    {
        addNewApp(vAppNames[i].data(), vAppIDs[i]);
    }
    flushListWidget();

}
