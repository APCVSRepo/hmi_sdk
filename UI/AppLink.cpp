#include "AppLink.h"
#include "Alert/AlertUI.h"
#include "ScrollableMessage/ScrollMsg.h"
#include "Show/Show.h"

extern Config g_config;

AppLink::AppLink(QWidget *parent)
    : AppBase(parent)
{

    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏


    m_scrollWidget.init(4, 280);
    connect(&m_scrollWidget,SIGNAL(upClicked()),this,SLOT(upArrowSlots()));
    connect(&m_scrollWidget,SIGNAL(downClicked()),this,SLOT(downArrowSlots()));

    connect(this,SIGNAL(moveBack()),this,SLOT(moveBackSlots()));

    initLayout();
}

AppLink::~AppLink()
{
    delete m_timer;
}

void AppLink::initLayout()
{
//    QVBoxLayout *btnLayout = new QVBoxLayout;
//    btnLayout->addStretch(1);
//    btnLayout->addWidget(&m_btn_FM,2, Qt::AlignCenter);
//    btnLayout->addWidget(&m_btn_Tel,2, Qt::AlignCenter);
//    btnLayout->addWidget(&m_btn_Msg,2, Qt::AlignCenter);
//    btnLayout->addWidget(&m_btn_CD,2, Qt::AlignCenter);
//    btnLayout->addWidget(&m_btn_List,2, Qt::AlignCenter);
//    btnLayout->addStretch(1);

    QHBoxLayout *midLayout = new QHBoxLayout;
    midLayout->addLayout(menuLayout,12);
    midLayout->addWidget(&m_listWidget,60);
    midLayout->addWidget(&m_scrollWidget,5);
    midLayout->addStretch(3);

    QVBoxLayout *mLayout = new QVBoxLayout(this);
    mLayout->addWidget(&m_lab_title,1, Qt::AlignCenter);
    mLayout->addLayout(midLayout, 4);
    mLayout->addWidget(&m_lab_time,1, Qt::AlignRight);

    mLayout->setMargin(0); //边框无缝
    mLayout->setSpacing(0);

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
    m_listWidget.verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 0px;}");
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

//    addNewApp("Sina");
//    addNewApp("Baidu");
//    addNewApp("Gaode");
}

void AppLink::addListItemWidget(QString text)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(QSize(g_config.m_i_itemW-10,g_config.m_i_itemH));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsDragEnabled);//不响应突出

    AppItemWidget *itemWidget = new AppItemWidget;
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
void AppLink::upArrowSlots()
{
    if(!m_b_downUp)
    {
        m_b_downUp = true;
        m_i_showRow -= 3;
    }

    if(m_i_currentRow > 1)
    {
        m_i_currentRow--;
        flushAllItems(m_i_currentRow);

        if(m_i_showRow > m_i_currentRow)
        {
            m_i_showRow -= 4;
            if(m_i_showRow <= 1)
                m_i_showRow = 1;
        }
        m_listWidget.setCurrentRow(m_i_showRow-1);
        m_scrollWidget.flushScroll(m_i_showRow,m_listWidget.count());
    }
}

//向下滚动
void AppLink::downArrowSlots()
{
    if(m_b_downUp)
    {
        m_b_downUp = false;
        m_i_showRow += 3;
    }

    if(m_i_currentRow < m_listWidget.count())
    {
        m_i_currentRow++;
        flushAllItems(m_i_currentRow);

        if(m_i_currentRow > m_i_showRow)
        {
            m_i_showRow +=4;
            if(m_i_showRow > m_listWidget.count())
                m_i_showRow = m_listWidget.count();
        }

        m_listWidget.setCurrentRow(m_i_showRow-1);
        m_scrollWidget.flushScroll(m_i_showRow-3,m_listWidget.count());
    }
}

void AppLink::moveBackSlots()
{
    this->close();
}

void AppLink::execShow(AppListInterface* pAppInterface)
{
    // TODO: getData();

    this->clearNewApp();
    if(NULL == pAppInterface)
    {
        this->show();
        return;
    }
    std::vector <Json::Value > pVecNewApp;
    pVecNewApp = pAppInterface->getNewAppJsonVector();
    for (int i = 0; i < pVecNewApp.size(); i++)
    {
        this->addNewApp(pVecNewApp.at(i)["params"]["application"]["appName"].asString().data()
                              ,pVecNewApp.at(i)["params"]["application"]["appID"].asInt());
    }
    this->show();
}
