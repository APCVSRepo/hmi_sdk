#include "Command.h"
#include "vector"
using std::vector;

Command::Command(AppListInterface * pList, QWidget *parent) : AppBase(pList, parent)
{
    m_listWidget = new AppListWidget(ui_app_width*0.1,0,ui_app_width*0.8,ui_app_height,this);
    initLayout();
   // connect(this,SIGNAL(returnShow()),this,SLOT(returnShowSlots()));
    connect(this,SIGNAL(exitApp()),this,SLOT(exitAppSlots()));
    connect(this,SIGNAL(commandClick(int)),this,SLOT(commandClickSlots(int)));
    connect(m_listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(listWidgetDoubleClickedSlots(QModelIndex)));
}

Command::~Command()
{
}

void Command::initLayout()
{
    QHBoxLayout *midLayout = new QHBoxLayout(this);
//    midLayout->addWidget(&m_btn_backIcon,12,Qt::AlignLeft);
    midLayout->addStretch(3);
    midLayout->addWidget(m_listWidget,60);
//    midLayout->addWidget(&m_scrollBar,5);
    midLayout->addStretch(3);

    m_btn_backIcon.setFlat(true);//就是这句能够实现按钮透明，用png图片时很有用
    m_btn_backIcon.setStyleSheet("border: 0px");//消除边框，取消点击效果
    connect(&m_btn_backIcon,SIGNAL(clicked()),this,SLOT(backBtnClickSlots()));
    hideBackIcon();

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




//双击某行，可进行某一行的子菜单;
void Command::listWidgetDoubleClickedSlots(QModelIndex index)
{
    static int no = 0;
    int indexNo = index.row();

    if(!m_b_backIcon)
    {
        if(m_listWidget->count() == index.row()+1)
        {
            emit exitApp();
            return;
        }

        if(m_vec_isMenu.at(indexNo))
        {
            for(int i = 0; i < m_vec_subStrList.size(); i++)
            {
                if(m_vec_cmdID.at(indexNo).str_command == m_vec_subStrList.at(i).at(0).str_command)
                {
                    no = i;
                    break;
                }
            }

            m_listWidget->DelListItemWidget();
            m_listWidget->SetScrollParams(4,m_vec_subStrList.at(no).size());
            for(int i = 1; i < m_vec_subStrList.at(no).size(); i++)
                m_listWidget->AddListItemWidget(m_vec_subStrList.at(no).at(i).str_command, false);

            m_listWidget->ItemSelect(indexNo);
            m_i_currentRow = indexNo;
            showBackIcon();
        }
        else
        {
            emit commandClick(m_vec_cmdID.at(indexNo).i_cmdID);
        }
    }
    else
    {
        emit commandClick(m_vec_subStrList.at(no).at(index.row()+1).i_cmdID);
    }
}

void Command::exitAppSlots()
{
    m_pList->OnAppExit();
}

void Command::commandClickSlots(int cmdID)
{
    //_D("appID = %d:%d\n",m_i_appID,cmdID);
    AppControl->OnCommandClick(cmdID);
}


void Command::clearAllCommand()
{
    m_vec_cmdID.clear();
    m_vec_isMenu.clear();
    m_vec_subStrList.clear();
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

    m_b_backIcon = false;
    //flushListWidget();
}
//增加Command，增加显示到list的最上一行;
void Command::addNewCommand(QString commandName, int cmdID)
{
    SCmdID tmpCmdID;
    tmpCmdID.str_command = commandName;
    tmpCmdID.i_cmdID = cmdID;

    m_vec_cmdID.insert(0,tmpCmdID);
    m_vec_isMenu.insert(0,false);

    //flushListWidget();
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

    m_listWidget->DelListItemWidget();
    m_listWidget->SetScrollParams(4,m_vec_cmdID.size());
    for(int i = 0; i < m_vec_cmdID.size(); i++)
    {
        m_listWidget->AddListItemWidget(m_vec_cmdID.at(i).str_command, m_vec_isMenu.at(i));
    }

	m_listWidget->FlushAll();
}



void Command::backBtnClickSlots()
{
    if(m_b_backIcon)
    {
        hideBackIcon();
        flushListWidget();
    }
}

void Command::showEvent(QShowEvent * e)
{
    if (AppControl)
    {
        this->clearAllCommand();
        vector<SMenuCommand> CmdList;
        CmdList = AppControl->getCommandList();
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
                TmpCmdList = AppControl->getCommandList(CmdList.at(i).i_menuID);
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
    flushListWidget();
}
