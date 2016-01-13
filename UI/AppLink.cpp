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

/*
void AppLink::addListItemWidget(QString text,QString strPath)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(QSize(m_listWidget.width(),ui_app_height/4.0));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsDragEnabled);//不响应突出

    AppItemWidget *itemWidget = new AppItemWidget(m_listWidget.width(),ui_app_height/4.0);
    m_listWidget.addItem(item);
    m_listWidget.setItemWidget(item,itemWidget);

    itemWidget->setText(text);
    itemWidget->SetIcon(strPath);
    m_vec_listItem.push_back(item);
    m_vec_appItemWidget.push_back(itemWidget);

    itemWidget->setRowNo(m_listWidget.count());
}

//刷新选中的白框在哪一行上;
void AppLink::flushAllItems(int currentNo)
{
    for(int i = 0; i < m_vec_appItemWidget.count(); i++)
        m_vec_appItemWidget.at(i)->flush(currentNo);
}
*/


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

void AppLink::addNewApp(QString appName,int appID,QString appIconPath)
{
    AppLinkItem linkItem;
    linkItem.appID=appID;
    linkItem.appName=appName;
    linkItem.strIconPath = appIconPath;
    m_appItems.insert(0,linkItem);
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
/*
    for(int i = 0; i < m_vec_listItem.size(); i++)
    {
        m_listWidget.removeItemWidget(m_vec_listItem.at(i));
        delete m_vec_listItem.at(i);
    }

    for(int i = 0; i < m_vec_appItemWidget.size(); i++)
    {
        if(m_vec_appItemWidget.at(i))
            delete m_vec_appItemWidget.at(i);
    }

    m_vec_listItem.clear();
    m_vec_appItemWidget.clear();
    m_listWidget.clear();

    for(int i = 0; i < m_vec_appList.size(); i++)
        //addListItemWidget(m_vec_appList.at(i).str_appName);
        addListItemWidget(m_vec_appList.at(i).str_appName,m_vec_appList.at(i).strIconPath);

    if(m_listWidget.count() > 4)
    {
        m_scrollWidget.show();
        m_scrollWidget.flushScroll(m_i_showRow,m_listWidget.count());
*/
    m_listWidget->DelListItemWidget();
    m_listWidget->SetScrollParams(4,m_appItems.count());
    for(int i=0;i<m_appItems.count();i++){
        m_listWidget->AddListItemWidget(m_appItems.at(i).appName,m_appItems.at(i).strIconPath,true);
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
    LOGI("---AppLink::showEvent");
    clearNewApp();
    setTitle("Mobile Apps");

    std::vector<int> vAppIDs;
    std::vector<std::string> vAppNames;
    std::vector<std::string> vIconPath;
    m_pList->getAppList(vAppIDs, vAppNames,vIconPath);
    for (unsigned int i = 0; i < vAppIDs.size(); i++)
    {
        addNewApp(vAppNames[i].data(), vAppIDs[i],vIconPath[i].c_str());
    }
    flushListWidget();
}
