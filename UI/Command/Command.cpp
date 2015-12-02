#include "Command.h"
#include "vector"
using std::vector;

Command::Command(QWidget *parent) : AppBase(parent)
{
    initLayout();
   // connect(this,SIGNAL(returnShow()),this,SLOT(returnShowSlots()));
    connect(this,SIGNAL(exitApp()),this,SLOT(exitAppSlots()));
    connect(this,SIGNAL(commandClick(int)),this,SLOT(commandClickSlots(int)));
}

Command::~Command()
{
}

void Command::initLayout()
{
    m_listWidget.setVerticalScrollBar(&m_scrollBar);
    QHBoxLayout *midLayout = new QHBoxLayout(this);
//    midLayout->addWidget(&m_btn_backIcon,12,Qt::AlignLeft);
    midLayout->addStretch(3);
    midLayout->addWidget(&m_listWidget,60);
//    midLayout->addWidget(&m_scrollBar,5);
    midLayout->addStretch(3);

m_listWidget.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//m_listWidget.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_btn_backIcon.setFlat(true);//就是这句能够实现按钮透明，用png图片时很有用
    m_btn_backIcon.setStyleSheet("border: 0px");//消除边框，取消点击效果
    connect(&m_btn_backIcon,SIGNAL(clicked()),this,SLOT(backBtnClickSlots()));
    hideBackIcon();

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

    SCmdID tmpCmdId;
    tmpCmdId.str_command = "Exit New application";
    tmpCmdId.i_cmdID = 0;
    m_vec_cmdID.insert(0,tmpCmdId);
    m_vec_isMenu.insert(0,true);

    for(int i = 0; i < m_vec_cmdID.size(); i++)
        addListItemWidget(m_vec_cmdID.at(i).str_command,m_vec_isMenu.at(i));

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

//    addNewCommand("Command1",1);
//    addNewMenu("Menu1",1);
//    addNewCommand("Command2",1);
//    addNewMenu("Menu3",1);
//    addNewMenu("Menu4",1);


//    addSubCommand("Menu1",1,"M1Commnad1",1);
//    addSubCommand("Menu1",1,"M1Commnad2",1);
//    addSubCommand("Menu1",1,"M1Commnad3",1);
//    addSubCommand("Menu1",1,"M1Commnad4",1);
//    addSubCommand("Menu1",1,"M1Commnad5",1);
//    addSubCommand("Menu1",1,"M1Commnad6",1);
}

void Command::hideBackIcon()
{
    m_b_backIcon = false;
//    QPixmap pixmap(":/images/appbar.chevron.left.png");
//    QPixmap fitpixmap=pixmap.scaled(76,76, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    m_btn_backIcon.setIcon(QIcon(fitpixmap));
//    m_btn_backIcon.setIconSize(QSize(0,0));
}
void Command::showBackIcon()
{
    m_b_backIcon = true;
//    QPixmap pixmap(":/images/appbar.chevron.left.png");
//    QPixmap fitpixmap=pixmap.scaled(76,76, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    m_btn_backIcon.setIcon(QIcon(fitpixmap));
//    m_btn_backIcon.setIconSize(QSize(76,76));
}

void Command::addListItemWidget(QString text, bool isMenu)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(QSize(ui_app_width*2/3.0,ui_app_height/4.0));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsDragEnabled);//不响应突出

    AppItemWidget *itemWidget = new AppItemWidget(ui_app_width*2/3.0,ui_app_height/4.0);
    m_listWidget.addItem(item);
    m_listWidget.setItemWidget(item,itemWidget);
    itemWidget->setIsMenu(isMenu);

    itemWidget->setText(text);
    m_vec_listItem.append(item);
    m_vec_appItemWidget.append(itemWidget);

    itemWidget->setRowNo(m_listWidget.count());
}

//刷新选中的白框在哪一行上;
void Command::flushAllItems(int currentNo)
{
    for(int i = 0; i < m_vec_appItemWidget.count(); i++)
        m_vec_appItemWidget.at(i)->flush(currentNo);
}

//单击某行，选中的那行的白框显示;
void Command::listWidgetClickedSlots(QModelIndex index)
{
    m_i_currentRow = index.row() + 1;
    flushAllItems(m_i_currentRow);
}

//双击某行，可进行某一行的子菜单;
void Command::listWidgetDoubleClickedSlots(QModelIndex index)
{
    static int no = 0;
    int indexNo = index.row();

    if(!m_b_backIcon)
    {
        if(m_listWidget.count() == index.row()+1)
        {
            emit exitApp();
            return;
        }

        if(m_vec_isMenu.at(indexNo))
        {
            qDebug()<<m_vec_cmdID.at(indexNo).str_command;
            for(int i = 0; i < m_vec_subStrList.size(); i++)
            {
                if(m_vec_cmdID.at(indexNo).str_command == m_vec_subStrList.at(i).at(0).str_command)
                {
                    no = i;
                    break;
                }
            }

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

            for(int i = 1; i < m_vec_subStrList.at(no).size(); i++)
                addListItemWidget(m_vec_subStrList.at(no).at(i).str_command, false);

            if(m_listWidget.count() > 4)
            {
                m_scrollBar.show();
                m_scrollBar.flushScroll(m_i_showRow,m_listWidget.count());
            }
            else
                m_scrollBar.hide();

            m_i_currentRow = 1;
            flushAllItems(m_i_currentRow);
            showBackIcon();
        }
        else
        {
            qDebug()<< m_vec_cmdID.at(indexNo).str_command<<"@@@"<<m_vec_cmdID.at(indexNo).i_cmdID;
            emit commandClick(m_vec_cmdID.at(indexNo).i_cmdID);
        }
    }
    else
    {
        qDebug()<< m_vec_subStrList.at(no).at(index.row()+1).str_command<<"###"<<m_vec_subStrList.at(no).at(index.row()+1).i_cmdID;
        emit commandClick(m_vec_subStrList.at(no).at(index.row()+1).i_cmdID);
    }
}

void Command::exitAppSlots()
{
    DataManager::ListInterface()->OnApplicationExit(DataManager::AppId());
    this->showCurUI(ID_APPLINK);
}

void Command::commandClickSlots(int cmdID)
{
    //_D("appID = %d:%d\n",m_i_appID,cmdID);
    DataManager::DataInterface()->OnCommandClick(DataManager::AppId(), cmdID);
}


void Command::clearAllCommand()
{
    m_vec_cmdID.clear();
    m_vec_isMenu.clear();
    m_vec_subStrList.clear();
    flushListWidget();

    SCmdID tmpCmdId;
    tmpCmdId.str_command = "Exit New application";
    tmpCmdId.i_cmdID = 0;
    m_vec_cmdID.insert(0,tmpCmdId);
    m_vec_isMenu.append(true);

    for(int i = 0; i < m_vec_cmdID.size(); i++)
        addListItemWidget(m_vec_cmdID.at(i).str_command, m_vec_isMenu.at(i));
}

//增加Menu，增加显示到list的最上一行;
void Command::addNewMenu(QString commandName, int menuID)
{
    SCmdID tmpCmdID;
    tmpCmdID.str_command = commandName;
    tmpCmdID.i_cmdID = menuID;
    m_vec_cmdID.insert(0, tmpCmdID);
    m_vec_isMenu.insert(0, true);

    SCmdID tmpMenuID;
    tmpMenuID.str_command = commandName;
    tmpMenuID.i_cmdID = menuID;
    QVector<SCmdID> tmpMenu;
    tmpMenu.append(tmpMenuID);
    m_vec_subStrList.insert(0,tmpMenu);

    qDebug()<< commandName<<menuID;
    m_b_backIcon = false;
    flushListWidget();
}
//增加Command，增加显示到list的最上一行;
void Command::addNewCommand(QString commandName, int cmdID)
{
    SCmdID tmpCmdID;
    tmpCmdID.str_command = commandName;
    tmpCmdID.i_cmdID = cmdID;

    m_vec_cmdID.insert(0,tmpCmdID);
    m_vec_isMenu.insert(0,false);

    flushListWidget();
}

//在Menu下增加子Command
void Command::addSubCommand(QString menu, int menuID, QString subCommand, int cmdID)
{
    for(int i = 0; i < m_vec_subStrList.size(); i++)
    {
        if(menuID == m_vec_subStrList.at(i).at(0).i_cmdID)
        {
            SCmdID tmpCmdID;
            tmpCmdID.str_command = subCommand;
            tmpCmdID.i_cmdID = cmdID;
            m_vec_subStrList[i].append(tmpCmdID);
        }
    }
}

//在Menu下删除子Command
void Command::delSubCommand(QString menu, QString subCommand)
{
    for(int i = 0; i < m_vec_subStrList.size(); i++)
    {
        if(menu == m_vec_subStrList.at(i).at(0).str_command)
        {
            for(int j = 0; j < m_vec_subStrList.at(i).size(); j++)
            {
                if(subCommand == m_vec_subStrList.at(i).at(j).str_command)
                    m_vec_subStrList[i].remove(j);
            }
        }
    }
}

//有APP下线，删除显示;
void Command::delMenuCommand(QString commandName)
{
    for(int i = 0; i < m_vec_cmdID.size(); i++)
    {
        if(m_vec_cmdID.at(i).str_command == commandName)
        {
            if(m_vec_isMenu.at(i))
            {
                for(int j = 0; j < m_vec_subStrList.size(); j++)
                {
                    if(commandName == m_vec_subStrList.at(j).at(0).str_command)
                        m_vec_subStrList.remove(j);
                }
            }
            m_vec_cmdID.remove(i);
            m_vec_isMenu.remove(i);
        }
    }

    flushListWidget();

}

//刷新list列表
void Command::flushListWidget()
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

    for(int i = 0; i < m_vec_cmdID.size(); i++)
    {
        addListItemWidget(m_vec_cmdID.at(i).str_command, m_vec_isMenu.at(i));
    }

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
void Command::upArrowSlots()
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
void Command::downArrowSlots()
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

void Command::backBtnClickSlots()
{
    if(m_b_backIcon)
    {
        hideBackIcon();
        flushListWidget();
    }
}

void Command::moveBackSlots()
{
    if(m_b_backIcon)
    {
        hideBackIcon();
        flushListWidget();
    }
    else
    {
        this->showCurUI(ID_SHOW);
    }
}

void Command::execShow()
{

    if (DataManager::DataInterface())
    {
        this->clearAllCommand();
        vector<SMenuCommand> CmdList;
        CmdList = DataManager::DataInterface()->getCommandList();
        for(int i = 0; i < CmdList.size(); i++)
        {
            if(0 != CmdList.at(i).i_cmdID && 0 == CmdList.at(i).i_menuID)
            {
                this->addNewCommand(CmdList.at(i).str_menuName.data()
                                          ,CmdList.at(i).i_cmdID);
//                this->setAppID(CmdList.at(i).i_appID);
            }
            else if(0 == CmdList.at(i).i_cmdID && 0 != CmdList.at(i).i_menuID)
            {
                this->addNewMenu(CmdList.at(i).str_menuName.data()
                                       ,CmdList.at(i).i_menuID);
//                this->setAppID(CmdList.at(i).i_appID);

                vector<SMenuCommand> TmpCmdList;
                TmpCmdList = DataManager::DataInterface()->getCommandList(CmdList.at(i).i_menuID);
                for(int j = 0; j < TmpCmdList.size(); j++)
                {
                    this->addSubCommand(CmdList.at(i).str_menuName.data()
                                              ,CmdList.at(i).i_menuID
                                              ,TmpCmdList.at(j).str_menuName.data()
                                              ,TmpCmdList.at(j).i_cmdID);
                }
            }
        }
    }
        addNewCommand("Command1",1);
        addNewMenu("Menu1",1);
        addNewCommand("Command2",1);
        addNewMenu("Menu3",1);
        addNewMenu("Menu4",1);

    this->show();
}
