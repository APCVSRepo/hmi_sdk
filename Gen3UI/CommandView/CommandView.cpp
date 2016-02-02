#include "CommandView.h"

CCommandView::CCommandView(AppListInterface *pList,QWidget *parent) :
    QWidget(parent),m_pCurrentMenu(nullptr)
{
    int iHeight = 0;
    if(parent)
    {
        iHeight = parent->height();
        setGeometry(0,0,parent->width(),iHeight);
    }
    m_pList = pList;

    setAutoFillBackground(true);
    QPixmap pixmap(":/images/MainWidget/Backgroud.png");
    pixmap = pixmap.scaled(width(),height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    setPalette(palette);

    m_pMainLayout = new QVBoxLayout(this);
    m_pTopLayout = new QHBoxLayout;
    m_pBottomLayout = new QHBoxLayout;
    m_pReturnBtn = new CButton;
    m_pAppNameLab = new QLabel;
    m_pCommandList = new CustomCombobox(iHeight*0.8);
    m_pCommandList->setMinimumWidth(width()*0.9);
    m_pCommandList->setMaximumWidth(width()*0.9);

    m_pTopLayout->addWidget(m_pReturnBtn);
    m_pTopLayout->addWidget(m_pAppNameLab);
    m_pTopLayout->addStretch(1);

    m_pBottomLayout->addStretch(1);
    m_pBottomLayout->addWidget(m_pCommandList);

    m_pMainLayout->addLayout(m_pTopLayout);
    m_pMainLayout->addLayout(m_pBottomLayout);

    m_pAppNameLab->setStyleSheet(QString("font: 60 40px \"Liberation Serif\";color:rgb(0,0,0);border: 0px"));

    m_pReturnBtn->initParameter(60,60,":/images/ReturnBtnNormal.png",":/images/ReturnBtnPress.png","","");
    connect(m_pReturnBtn,SIGNAL(clicked()),this,SLOT(OnReturnBtnClicked()));

    connect(m_pCommandList,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(OnCommandListItemClicked(QListWidgetItem *)));
}

CCommandView::~CCommandView()
{
    delete m_pMainLayout;
    delete m_pReturnBtn;
    delete m_pAppNameLab;
    delete m_pCommandList;
}

void CCommandView::AddCommand(int iCmdId,std::string strName)
{
    tagCmdInf tempCmdInf;
    tempCmdInf.strCmd = strName;
    tempCmdInf.bMenu = false;
    tempCmdInf.iId = iCmdId;
    tempCmdInf.iParentId = 0;
    m_CmdVec.push_back(tempCmdInf);
}

void CCommandView::AddMenu(int iMenuId,std::string strName)
{
    tagCmdInf tempCmdInf;
    tempCmdInf.strCmd = strName;
    tempCmdInf.bMenu = true;
    tempCmdInf.iId = iMenuId;
    tempCmdInf.iParentId = 0;
    m_CmdVec.push_back(tempCmdInf);
}

void CCommandView::AddSubCommand(int iParentId,int iCmdId,std::string strName)
{
    tagCmdInf tempCmdInf;
    tempCmdInf.strCmd = strName;
    tempCmdInf.bMenu = false;
    tempCmdInf.iId = iCmdId;
    tempCmdInf.iParentId = iParentId;
    for(int i = 0;i != m_CmdVec.size();++i)
    {
        if(m_CmdVec[i].bMenu && m_CmdVec[i].iId == iParentId)
        {
            m_CmdVec[i].CmdVec.push_back(tempCmdInf);
            return;
        }
    }
}

void CCommandView::RefreshCommandList(tagCmdInf *pMenu)
{
    m_pCommandList->ClearAllItem();

    if(pMenu == nullptr)
    {
        for(int i = 0;i != m_CmdVec.size();++i)
        {
            m_pCommandList->AddListItem(m_CmdVec[i].strCmd.c_str(),m_CmdVec[i].bMenu);
        }
    }
    else
    {
        for(int i = 0;i != pMenu->CmdVec.size();++i)
        {
            m_pCommandList->AddListItem(pMenu->CmdVec[i].strCmd.c_str(),pMenu->CmdVec[i].bMenu);
        }
    }
    m_pCommandList->show();
}

void CCommandView::showEvent(QShowEvent * e)
{
    //m_pCommandList->SetPos(m_pMenuBtn->geometry().left(),m_pMenuBtn->geometry().top(),m_pMenuBtn->geometry().width()*2.5,0);
    m_CmdVec.clear();

    std::vector<SMenuCommand> CmdList = m_pList->getActiveApp()->getCommandList();
    std::vector<SMenuCommand> TmpCmdList;
    for(unsigned int i = 0; i < CmdList.size(); i++)
    {
        if(0 != CmdList.at(i).i_cmdID && 0 == CmdList.at(i).i_menuID)
        {
            AddCommand(CmdList.at(i).i_cmdID,CmdList.at(i).str_menuName);
        }
        else if(0 == CmdList.at(i).i_cmdID && 0 != CmdList.at(i).i_menuID)
        {
            AddMenu(CmdList.at(i).i_menuID,CmdList.at(i).str_menuName);

            TmpCmdList = m_pList->getActiveApp()->getCommandList(CmdList.at(i).i_menuID);
            for(unsigned int j = 0; j < TmpCmdList.size(); j++)
            {
                AddSubCommand(CmdList.at(i).i_menuID,TmpCmdList.at(j).i_cmdID,TmpCmdList.at(j).str_menuName.data());
            }
        }
    }
    m_pCurrentMenu = nullptr;
    RefreshCommandList(0);
}

void CCommandView::OnReturnBtnClicked()
{
    m_pList->getActiveApp()->OnCommandClick(-1);
}
