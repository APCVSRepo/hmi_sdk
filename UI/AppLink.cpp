#include "AppLink.h"
#include "UI/Notify/Notify.h"
#include "Common/AppBase.h"
AppLink::AppLink(AppListInterface * pList, QWidget *parent)
    : AppBase(pList, parent)
{
    m_scrollWidget.init(4, ui_app_height-30);
    connect(this,SIGNAL(inAppSignals(int)),this,SLOT(inAppSlots(int)));
    connect(this,SIGNAL(findNewApp()),this,SLOT(findNewAppSlots()));
    initLayout();
}

AppLink::~AppLink()
{
}

void AppLink::initLayout()
{
    m_listWidget.setVerticalScrollBar(&m_scrollWidget);
    m_listWidget.setParent(this);
    m_listWidget.setGeometry(ui_app_width*0.1,0,ui_app_width*0.8,ui_app_height);
    m_listWidget.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

#ifndef ANDROID
    QPalette pll = m_listWidget.palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
    m_listWidget.setPalette(pll);
#endif

    m_listWidget.setFrameShape(QFrame::NoFrame); //设置无边框
    m_listWidget.setFocusPolicy(Qt::NoFocus); //去除选中虚线框
    m_listWidget.setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
#ifdef ANDROID
    m_listWidget.setStyleSheet("background-color:transparent");
#else
    m_listWidget.setStyleSheet("QListWidget:item:hover{border: 0px;}"); //鼠标移上去不响应突出
#endif

    connect(&m_listWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(listWidgetClickedSlots(QModelIndex)));
    connect(&m_listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(listWidgetDoubleClickedSlots(QModelIndex)));

    m_i_currentRow = 1;
    flushAllItems(m_i_currentRow);

    m_b_downUp = true;
    m_i_showRow = 1;

    if(m_listWidget.count() > 4)
    {
        m_scrollWidget.show();
        m_scrollWidget.flushScroll(m_i_showRow,m_listWidget.count());
    }
    else
        m_scrollWidget.hide();

}

void AppLink::addListItemWidget(QString text)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(QSize(m_listWidget.width(),ui_app_height/4.0));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsDragEnabled);//不响应突出

    AppItemWidget *itemWidget = new AppItemWidget(m_listWidget.width(),ui_app_height/4.0);
    m_listWidget.addItem(item);
    m_listWidget.setItemWidget(item,itemWidget);

    itemWidget->setText(text);
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

//单击某行，选中的那行的白框显示;
void AppLink::listWidgetClickedSlots(QModelIndex index)
{
    m_i_currentRow = index.row() + 1;
    flushAllItems(m_i_currentRow);
}

//双击某行，可进行某一行的子菜单;
void AppLink::listWidgetDoubleClickedSlots(QModelIndex index)
{
    int appID = m_vec_appList.at(index.row()).i_appID;

    if(appID > 1)
        emit inAppSignals(appID);
    else if(index.row()+1 == m_listWidget.count())
    {
        //App Setting
        emit appSetting();
    }
    else if(index.row()+2 == m_listWidget.count())
    {
        //Find New App
        emit findNewApp();
    }
}

//有新的APP上线，增加显示到list的最上一行;
void AppLink::addNewApp(QString appName, int appID)
{
//    m_vec_string.insert(0,appTitle);
    SAppList tmpAppList;
    tmpAppList.str_appName = appName;
    tmpAppList.i_appID = appID;

    m_vec_appList.insert(0, tmpAppList);

    flushListWidget();
}

//有APP下线，删除显示;
void AppLink::delNewApp(QString appName)
{
    for(int i = 0; i < m_vec_appList.size(); i++)
    {
        if(m_vec_appList.at(i).str_appName == appName)
            m_vec_appList.remove(i);
    }

    flushListWidget();
}

void AppLink::clearNewApp()
{

    m_vec_appList.clear();
    flushListWidget();

    SAppList tmpAppSetting;
    tmpAppSetting.str_appName = "App settings";
    tmpAppSetting.i_appID = 0;
    m_vec_appList.insert(0, tmpAppSetting);

    SAppList tmpFindNewApp;
    tmpFindNewApp.str_appName = "Find new Apps";
    tmpFindNewApp.i_appID = 0;
    m_vec_appList.insert(0, tmpFindNewApp);


    for(int i = 0; i < m_vec_appList.size(); i++)
        addListItemWidget(m_vec_appList.at(i).str_appName);

}

//刷新list列表
void AppLink::flushListWidget()
{
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
        addListItemWidget(m_vec_appList.at(i).str_appName);

    if(m_listWidget.count() > 4)
    {
        m_scrollWidget.show();
        m_scrollWidget.flushScroll(m_i_showRow,m_listWidget.count());
    }
    else
        m_scrollWidget.hide();

    flushAllItems(m_i_currentRow);
}

//向上滚动
void AppLink::onScrollTo(int v)
{

}



void AppLink::inAppSlots(int appID)
{
    m_pList->OnAppActivated(appID);
}



void AppLink::findNewAppSlots()
{
    Notify n;
    n.show();
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
}
