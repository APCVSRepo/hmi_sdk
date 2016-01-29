#include "MainWidget.h"
//#include "../Config/Config.h"


#define MARGIN 10

MainWidget::MainWidget(AppListInterface * pList, QWidget *parent) : QWidget(parent)
{    
    if(parent)
    {
        setGeometry(0,0,parent->width(),parent->height());
    }
    m_pList = pList;

    setAutoFillBackground(true);
    QPixmap pixmap(":/images/MainWidget/Backgroud.png");
    pixmap = pixmap.scaled(width(),height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    setPalette(palette);

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);

    QVBoxLayout *pTopLayout = new QVBoxLayout;
    pTopLayout->setSpacing(1);
    pTopLayout->setMargin(MARGIN);
    m_pAppNameLab = new QLabel;
    pTopLayout->addWidget(m_pAppNameLab,0,Qt::AlignLeft);
    m_pAppNameLab->setStyleSheet(QString("font: 60 40px \"Liberation Serif\";color:rgb(0,0,0);border: 0px"));

    m_pShowLine = new QLabel[4];
    for(int i = 0;i != 4;++i)
    {
        pTopLayout->addWidget(m_pShowLine + i,0,Qt::AlignLeft);
        m_pShowLine[i].setStyleSheet("border:0px;font: 10px \"Liberation Serif\";color:rgb(0,0,0)");
    }

    QHBoxLayout *pCenterLayout = new QHBoxLayout;
    m_pIconLab = new QLabel;
    m_pMainTitleLab = new QLabel;
    pCenterLayout->addStretch(1);
    pCenterLayout->addWidget(m_pIconLab,0,Qt::AlignCenter);
    pCenterLayout->addWidget(m_pMainTitleLab,0,Qt::AlignCenter);
    pCenterLayout->addStretch(1);
    m_pMainTitleLab->setStyleSheet(QString("font: 60 35pt \"Liberation Serif\";color:rgb(0,0,0);border: 0px"));
    m_pMainTitleLab->setText("SmartDeviceLink");
    QPixmap IconPixmap(":/images/MainWidget/AppIcon.png");
    IconPixmap = IconPixmap.scaled(80,80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_pIconLab->setPixmap(IconPixmap);

    QVBoxLayout *pBottomLayout = new QVBoxLayout;
    QHBoxLayout *pBtnLayout = new QHBoxLayout;
    pBtnLayout->setSpacing(0);
    pBtnLayout->setMargin(MARGIN);
    m_pMenuBtn = new CButton;
    m_pMenuBtn->initParameter(ui_btn_width,ui_btn_height,":/images/MainWidget/BtnNormal.png",":/images/MainWidget/BtnPress.png","","-");
    m_pMenuBtn->setTextStyle("border:0px;font: 36px \"Liberation Serif\";color:rgb(0,0,0)");
    m_pMenuBtn->SetLeftIcon(":/images/MainWidget/MenuIcon.png");
    m_pSoftBtn = new CButton[3];
    pBtnLayout->addWidget(m_pMenuBtn,0,Qt::AlignLeft);
    pBottomLayout->addStretch(1);
    pBottomLayout->addLayout(pBtnLayout);

    for(int i = 0;i != 3;++i)
    {
        pBtnLayout->addWidget(&m_pSoftBtn[i],0,Qt::AlignLeft);
        m_pSoftBtn[i].initParameter(ui_btn_width,ui_btn_height,":/images/MainWidget/BtnNormal.png",":/images/MainWidget/BtnPress.png","","-");
        m_pSoftBtn[i].setTextStyle("border:0px;font: 36px \"Liberation Serif\";color:rgb(0,0,0)");
    }
    pBtnLayout->addStretch(1);

    pMainLayout->addLayout(pTopLayout,1);
    pMainLayout->addLayout(pCenterLayout);
    pMainLayout->addLayout(pBottomLayout,1);

    m_pMenuBtn->setText("Menu");
    for(int i = 0;i != 3;++i)
    {
        m_pSoftBtn[i].setText("-");
        connect(&m_pSoftBtn[i],SIGNAL(clicked(int)),this,SLOT(SoftBtnClickedSlot(int)));
    }

    connect(m_pMenuBtn,SIGNAL(clicked()),this,SLOT(BtnMenuClickedSlots()));

    //m_pCommandList = new CustomCombobox(height()*0.6,true,this);
    //m_pCommandList->hide();
    //connect(m_pCommandList,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(OnCommandListItemClicked(QListWidgetItem *)));
}

MainWidget::~MainWidget()
{
    delete m_pAppNameLab;
    delete m_pIconLab;
    delete m_pMainTitleLab;

    delete []m_pSoftBtn;
}

void MainWidget::SetAppName(QString strName)
{
    AppBase::SetEdlidedText(m_pAppNameLab,strName,width()*0.9);
}

void MainWidget::BtnMenuClickedSlots()
{
    m_pList->getActiveApp()->OnShowCommand();
    /*
    m_pCommandList->SetPos(m_pMenuBtn->geometry().left(),m_pMenuBtn->geometry().top(),m_pMenuBtn->geometry().width()*2.5,0);
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
    */
}

/*
void MainWidget::AddCommand(int iCmdId,std::string strName)
{
    tagCmdInf tempCmdInf;
    tempCmdInf.strCmd = strName;
    tempCmdInf.bMenu = false;
    tempCmdInf.iId = iCmdId;
    tempCmdInf.iParentId = 0;
    m_CmdVec.push_back(tempCmdInf);
}

void MainWidget::AddMenu(int iMenuId,std::string strName)
{
    tagCmdInf tempCmdInf;
    tempCmdInf.strCmd = strName;
    tempCmdInf.bMenu = true;
    tempCmdInf.iId = iMenuId;
    tempCmdInf.iParentId = 0;
    m_CmdVec.push_back(tempCmdInf);
}

void MainWidget::AddSubCommand(int iParentId,int iCmdId,std::string strName)
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


void MainWidget::RefreshCommandList(tagCmdInf *pMenu)
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
*/
void MainWidget::SoftBtnClickedSlot(int iSoftBtnID)
{
    if(iSoftBtnID != 0)
    {
        m_pList->getActiveApp()->OnSoftButtonClick(iSoftBtnID, 1);
    }
}

void MainWidget::mousePressEvent(QMouseEvent * event)
{
    //HideCommandList();
}

void MainWidget::OnCommandListItemClicked(QListWidgetItem *pItem)
{
    /*
    int iRow = m_pCommandList->row(pItem);
    HideCommandList();

    if(m_pCurrentMenu == nullptr)
    {
        if(iRow == 0)
        {
            //emit ExitApp();
            m_pList->OnAppExit();
        }
        else
        {
            if(m_CmdVec[iRow].bMenu)
            {
                m_pCurrentMenu = &m_CmdVec[iRow];
                RefreshCommandList(m_pCurrentMenu);
            }
            else
            {
                //emit CommandClick(m_CmdVec[iRow].iId);
                m_pList->getActiveApp()->OnCommandClick(m_CmdVec[iRow].iId);
            }
        }
    }
    else
    {
        //emit CommandClick(m_pCurrentMenu->CmdVec[iRow].iId);
        m_pList->getActiveApp()->OnCommandClick(m_pCurrentMenu->CmdVec[iRow].iId);
    }
    */
}
/*
void MainWidget::HideCommandList()
{

    m_pCommandList->ClearAllItem();
    m_pCommandList->hide();

}
*/
void MainWidget::setSoftButtons(std::vector<SSoftButton> vec_softButtons)
{
    int iBtnCount = vec_softButtons.size() > 3?3:vec_softButtons.size();
    for(int i = 0;i != iBtnCount;++i)
    {
        m_pSoftBtn[i].setText(vec_softButtons[i].str_text.c_str());
    }
}

void MainWidget::showEvent(QShowEvent * e)
{
    for(int i = 0;i != 4;++i)
    {
        m_pShowLine[i].setText("");
    }
    for(int i = 0;i != 3;++i)
    {
        m_pSoftBtn[i].setText("-");
    }

    Json::Value pObj;
    std::vector <SSoftButton > vec_softButtons;
    vec_softButtons.clear();
    if (m_pList->getActiveApp())
    {
        SetAppName(m_pList->getActiveApp()->getAppName().c_str());

        pObj = m_pList->getActiveApp()->getShowData();
        if(pObj.isNull())
            return;
        Json::Value jsonParams = pObj["params"];        

        for (unsigned int i = 0; i < jsonParams["showStrings"].size(); i++)
        {
            Json::Value  fieldName = jsonParams["showStrings"][i];
            if ("mainField1" == fieldName["fieldName"].asString())
            {
                AppBase::SetEdlidedText(m_pShowLine,fieldName["fieldText"].asString().c_str(),width()*0.9);
            }
            else if ("mainField2" == fieldName["fieldName"].asString())
            {
                AppBase::SetEdlidedText(m_pShowLine+1,fieldName["fieldText"].asString().c_str(),width()*0.9);
            }
            else if ("mainField3" == fieldName["fieldName"].asString())
            {
                AppBase::SetEdlidedText(m_pShowLine+2,fieldName["fieldText"].asString().c_str(),width()*0.9);
            }
            else if ("mainField4" == fieldName["fieldName"].asString())
            {
                AppBase::SetEdlidedText(m_pShowLine+3,fieldName["fieldText"].asString().c_str(),width()*0.9);
            }
        }

        if (jsonParams.isMember("softButtons"))
        {
            for (int i = 0; i < jsonParams["softButtons"].size(); i++)
            {
                SSoftButton tmpSoftButton;
                tmpSoftButton.b_isHighlighted = jsonParams["softButtons"][i]["isHighlighted"].asBool();
                tmpSoftButton.i_softButtonID = jsonParams["softButtons"][i]["softButtonID"].asInt();
                tmpSoftButton.str_text = jsonParams["softButtons"][i]["text"].asString();
                vec_softButtons.push_back(tmpSoftButton);
            }
            setSoftButtons(vec_softButtons);
        }
    }
}
