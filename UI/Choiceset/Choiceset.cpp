#include "Choiceset.h"

Choiceset::Choiceset(AppListInterface * pList, QWidget *parent) : AppBase(pList, parent)
{

    m_timerHide = new QTimer;

    initLayout();
    connect(this,SIGNAL(menuClicked(int,int,int)),this,SLOT(menuClickedSlots(int,int,int)));
}

Choiceset::~Choiceset()
{

}

void Choiceset::initLayout()
{
    m_listWidget.setVerticalScrollBar(&m_scrollBar);
    QHBoxLayout *midLayout = new QHBoxLayout;
    midLayout->addStretch(2);
    midLayout->addWidget(&m_listWidget,65);

    midLayout->addStretch(3);
    m_listWidget.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *mLayout = new QVBoxLayout(this);
    mLayout->addLayout(midLayout, 4);

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
    //m_listWidget.verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 0px;}");
    connect(&m_listWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(listWidgetClickedSlots(QModelIndex)));
    connect(&m_listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(listWidgetDoubleClickedSlots(QModelIndex)));

    m_scrollBar.init(4, ui_app_height-30);
//    connect(&m_scrollBar,SIGNAL(upClicked()),this,SLOT(upArrowSlots()));
//    connect(&m_scrollBar,SIGNAL(downClicked()),this,SLOT(downArrowSlots()));
    m_scrollBar.hide();
    m_i_currentRow = 1;
    flushAllItems(m_i_currentRow);

    m_b_downUp = true;
    m_i_showRow = 1;

    if(m_listWidget.count() > 4)
    {
        m_scrollBar.show();
        m_scrollBar.flushScroll(m_i_showRow,m_listWidget.count());
    }
    else
        m_scrollBar.hide();
}

void Choiceset::addListItemWidget(QString text, bool isMenu)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(QSize(ui_item_width,ui_item_height));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsDragEnabled);//不响应突出

    AppItemWidget *itemWidget = new AppItemWidget(ui_item_width,ui_item_height);
    m_listWidget.addItem(item);
    m_listWidget.setItemWidget(item,itemWidget);
    itemWidget->setIsMenu(isMenu);

    itemWidget->setText(text);
    m_vec_listItem.append(item);
    m_vec_appItemWidget.append(itemWidget);

    itemWidget->setRowNo(m_listWidget.count());
}

//刷新选中的白框在哪一行上;
void Choiceset::flushAllItems(int currentNo)
{
    for(int i = 0; i < m_vec_appItemWidget.count(); i++)
        m_vec_appItemWidget.at(i)->flush(currentNo);
}

//单击某行，选中的那行的白框显示;
void Choiceset::listWidgetClickedSlots(QModelIndex index)
{
    m_i_currentRow = index.row() + 1;
    flushAllItems(m_i_currentRow);
}

//双击某行，可进行某一行的子菜单;
void Choiceset::listWidgetDoubleClickedSlots(QModelIndex index)
{
    m_timerHide->stop();
    emit menuClicked(PERFORMINTERACTION_CHOICE, m_vec_choiceMenu.at(index.row()).i_choiceID);
    this->hide();
}

//增加Command，增加显示到list的最上一行;
void Choiceset::addNewMenu(int choiceID, std::string menuName)
{
    SChoiceMenu tmpChoiceMenu;
    tmpChoiceMenu.i_choiceID = choiceID;
    tmpChoiceMenu.str_menuName = menuName;
    m_vec_choiceMenu.insert(0,tmpChoiceMenu);
    m_vec_isMenu.insert(0,false);

    flushListWidget();
}

//刷新list列表
void Choiceset::flushListWidget()
{
    for(int i = 0; i < m_vec_listItem.size(); i++)
    {
        m_listWidget.removeItemWidget(m_vec_listItem.at(i));
        delete m_vec_listItem.at(i);
    }
    for(int i = 0; i < m_vec_appItemWidget.size(); i++)
    {
        delete m_vec_appItemWidget.at(i);
    }
    m_vec_listItem.clear();
    m_vec_appItemWidget.clear();
    m_listWidget.clear();

    for(int i = 0; i < m_vec_choiceMenu.size(); i++)
        addListItemWidget(m_vec_choiceMenu.at(i).str_menuName.data(), m_vec_isMenu.at(i));

    if(m_listWidget.count() > 4)
    {
        m_scrollBar.show();
        m_scrollBar.flushScroll(m_i_showRow,m_listWidget.count());
    }
    else
        m_scrollBar.hide();

    flushAllItems(m_i_currentRow);
}

//向上滚动
void Choiceset::upArrowSlots()
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
        m_scrollBar.flushScroll(m_i_showRow,m_listWidget.count());
    }
}

//向下滚动
void Choiceset::downArrowSlots()
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
        m_scrollBar.flushScroll(m_i_showRow-3,m_listWidget.count());
    }
}

void Choiceset::menuClickedSlots(int code, int choiceID)
{
    //_D("code=%d:%d:%d\n",code,performInteractionID,choiceID);
    m_pList->getActiveApp()->OnPerformInteraction(code, choiceID);
}

void Choiceset::setChoicesetName(QString title)
{
    this->setTitle(title);
}


void Choiceset::setTimeOut(int duration)
{
    m_timerHide->start(duration);

}

void Choiceset::timeHideOutSlots()
{
    m_timerHide->stop();
    emit menuClicked(PERFORMINTERACTION_TIMEOUT, m_vec_choiceMenu.at(0).i_choiceID);
}

void Choiceset::showEvent(QShowEvent * e)
{
    setChoicesetName("Choice Name");
    if (m_pList->getActiveApp())
    {
        Json::Value jsonData = m_pList->getActiveApp()->getInteractionJson();
        setTimeOut(jsonData["params"]["timeout"].asInt());
        for(int i = 0; i < jsonData["params"]["choiceSet"].size(); i++)
        {
            addNewMenu(jsonData["params"]["choiceSet"][i]["choiceID"].asInt(),jsonData["params"]["choiceSet"][i]["menuName"].asString());
        }
    }
}
