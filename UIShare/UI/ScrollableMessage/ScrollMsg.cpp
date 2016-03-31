#include "ScrollMsg.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "UI/Config/Config.h"
#include <QTextLayout>
#include <QTextBlock>
#include "Common/ScrollBar.h"

CScrollMsg::CScrollMsg(AppListInterface * pList, QWidget *parent) : AppBase(pList, parent)
{
    InitLayout();
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timeoutSlots()));
    connect(this, SIGNAL(onSpaceCliced()), this, SLOT(onSpaceClicedSlots()));
    connect(this,SIGNAL(scrollMsgAbort(int)),this,SLOT(scrollMsgAbortSlots(int)));
}

CScrollMsg::~CScrollMsg()
{

}

void CScrollMsg::InitLayout()
{
    m_listWidget=new AppListWidget(ui_app_width*0.1,0,ui_app_width*2.0/3.0,ui_app_height,this);
    m_editText = new QTextEdit(this);
    char* image[4]={":/images/softbutton_alert.png",
                   ":/images/softbutton_alert_left.png",
                   ":/images/softbutton_alert_right.png",
                   ":/images/softbutton_alert.png"};
    char* text[4]={"Soft1","Soft2","Soft3","Soft4"};
    for(int i=0;i<4;i++){
        m_btnSoft[i]=new CButton(this);
        m_btnSoft[i]->initParameter(ui_btn_width,ui_aler_height,image[i],image[i],"",text[i]);
        m_btnSoft[i]->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    }

    connect(m_listWidget,SIGNAL(clicked(int)),this,SLOT(onItemClicked(int)));
    connect(m_listWidget,SIGNAL(longclicked(int)),this,SLOT(onItemLongClicked(int)));

    QPalette pll = m_editText->palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    m_editText->setPalette(pll);
//    m_editText->setFixedSize(600,250);
    m_editText->setAttribute(Qt::WA_TranslucentBackground, true);
    //m_editText->setReadOnly(true); //设置不可编辑
    m_editText->setFrameShape(QFrame::NoFrame); //设置无边框
    m_editText->setStyleSheet(ScrollBar::cssString()+"border:1px;background-color:white;color:grey;font:36px \"Liberation Serif\";");

    m_editText->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

   // m_listWidget->hide();

    m_listWidget->setFixedSize(ui_app_width*2/3,ui_app_height);

}

void CScrollMsg::UpdateLayout()
{
    if(m_listButton.size()<=4){
        for(int i=0;i<m_listButton.size();i++){
            setButtonStyle(i,m_listButton.at(i).btnId,m_listButton.at(i).btnText,
                           m_listButton.at(i).isLighted);
        }
        for(int i=m_listButton.size();i<4;i++){
            m_btnSoft[i]->setText("-");
        }
    }
    else{
        for(int i=0;i<3;i++){
            setButtonStyle(i,m_listButton.at(i).btnId,m_listButton.at(i).btnText,
                           m_listButton.at(i).isLighted);
        }
        m_btnSoft[3]->setText("More");

        m_listWidget->DelListItemWidget();
        m_listWidget->SetScrollParams(4,m_listButton.size());
        for(int i=0;i<m_listButton.size();i++){
            m_listWidget->AddListItemWidget(m_listButton.at(i).btnText,false);
        }

    }
    ChangeLayout(0);
}

void CScrollMsg::delLayout(QLayout *layout)
{
    if(layout==NULL)
        return;
    int count=layout->count();
    if(count==0)
        return;
    for(int i=count-1;i>=0;i--){
        QLayoutItem *item=layout->itemAt(i);
        delLayout(item->layout());
        layout->removeItem(item);
    }
}

void CScrollMsg::ChangeLayout(int flag)
{
    //delLayout(m_pMainLayout);
    if(flag==0){
        m_editText->show();
        m_editText->setGeometry(10,10,ui_app_width-20,ui_app_height*3/4-15);
        for(int i=0;i<4;i++){
            m_btnSoft[i]->show();
            m_btnSoft[i]->setGeometry(5+i*ui_btn_width,ui_app_height*3/4+15,ui_btn_width,ui_app_height*1/4-20);
        }
        m_listWidget->hide();
//        QHBoxLayout midLayout;
//        midLayout.addStretch(5);
//        midLayout.addWidget(m_editText, 85);
//        midLayout.addStretch(10);

//        QHBoxLayout bottomLayout;
//        bottomLayout.addWidget(m_btnSoft[0]);
//        bottomLayout.addWidget(m_btnSoft[1]);
//        bottomLayout.addWidget(m_btnSoft[2]);
//        bottomLayout.addWidget(m_btnSoft[3]);


//        m_pMainLayout->addStretch(2);
//       // m_pMainLayout->addLayout(upLayout, 7);
//    //    mLayout->addWidget(m_editText, 66, Qt::AlignCenter);
//        m_pMainLayout->addLayout(&midLayout, 61);
//        m_pMainLayout->addStretch(2);
//        m_pMainLayout->addLayout(&bottomLayout, 20);
//        m_pMainLayout->addStretch(1);
//        m_pMainLayout->setMargin(0);
       // this->setLayout(&mainLayout);
    }
    else{
        m_listWidget->show();
        m_editText->hide();
        for(int i=0;i<4;i++){
            m_btnSoft[i]->hide();
        }
//        m_pMainLayout->addStretch(4);
//        m_pMainLayout->addWidget(m_listWidget,92,Qt::AlignCenter);
//        m_pMainLayout->addStretch(4);
    }
}


void CScrollMsg::setTimeOut(int duration)
{
    m_timer.start(duration);

}

void CScrollMsg::timeoutSlots()
{
    m_timer.stop();
    emit scrollMsgAbort(0);
}

void CScrollMsg::setMessage(QString msg)
{
    m_editText->setText(msg);
}

void CScrollMsg::setButtonStyle(int index,int btnId, QString text, bool highLight)
{
    switch (index)
    {
    case 0:
    {
        m_btnSoft[0]->setId(btnId);
        m_btnSoft[0]->setText(text);
        if(highLight)
        {
            m_btnSoft[0]->setIconNormal(":/images/highlightsoftbutton_alert.png");
            m_btnSoft[0]->setIconPressed(":/images/highlightsoftbutton_alert.png");
        }
        else
        {
            m_btnSoft[0]->setIconNormal(":/images/softbutton_alert.png");
            m_btnSoft[0]->setIconPressed(":/images/softbutton_alert.png");
        }
    }
        break;
    case 1:
    {
        m_btnSoft[1]->setId(btnId);
        m_btnSoft[1]->setText(text);
        if(highLight)
        {
            m_btnSoft[1]->setIconNormal(":/images/highlightsoftbutton_alert_left.png");
            m_btnSoft[1]->setIconPressed(":/images/highlightsoftbutton_alert_left.png");
        }
        else
        {
            m_btnSoft[1]->setIconNormal(":/images/softbutton_alert_left.png");
            m_btnSoft[1]->setIconPressed(":/images/softbutton_alert_left.png");
        }
    }
        break;
    case 2:
    {
        m_btnSoft[2]->setId(btnId);
        m_btnSoft[2]->setText(text);
        if(highLight)
        {
            m_btnSoft[2]->setIconNormal(":/images/highlightsoftbutton_alert_right.png");
            m_btnSoft[2]->setIconPressed(":/images/highlightsoftbutton_alert_right.png");
        }
        else
        {
            m_btnSoft[2]->setIconNormal(":/images/softbutton_alert_right.png");
            m_btnSoft[2]->setIconPressed(":/images/softbutton_alert_right.png");
        }
    }
        break;
    case 3:
    {
        m_btnSoft[3]->setId(btnId);
        m_btnSoft[3]->setText(text);
        if(highLight)
        {
            m_btnSoft[3]->setIconNormal(":/images/highlightsoftbutton_alert.png");
            m_btnSoft[3]->setIconPressed(":/images/highlightsoftbutton_alert.png");
        }
        else
        {
            m_btnSoft[3]->setIconNormal(":/images/softbutton_alert.png");
            m_btnSoft[3]->setIconPressed(":/images/softbutton_alert.png");
        }
    }
        break;
    }
}

void CScrollMsg::addSoftButton(int btnId, QString text, bool highLight)
{
    SoftButton button;
    button.btnId=btnId;
    button.btnText=text;
    button.isLighted=highLight;
    m_listButton.append(button);
}


void CScrollMsg::onSpaceClicedSlots()
{
    m_timer.stop();
    emit scrollMsgAbort(2);
}

void CScrollMsg::onButtonClickedSlots(int btID)
{
	if (m_listButton.size()>4){
		CButton *button = static_cast<CButton*>(sender());
		if (m_btnSoft[3] == button){
            ChangeLayout(1);
			return;
		}
	}

    m_timer.stop();
    emit scrollMsgAbort(1);
    if(btID != 0)
    {
        AppControl->OnSoftButtonClick(btID, 0);
    }
}

void CScrollMsg::onButtonClickedLongSlots(int btID)
{
    if(m_listButton.size()>4){
       CButton *button=static_cast<CButton*>(sender());
       if(m_btnSoft[3]==button){
           ChangeLayout(1);
           return;
       }
    }


    m_timer.stop();
    emit scrollMsgAbort(1);
    if(btID != 0)
    {
        AppControl->OnSoftButtonClick(btID, 1);
    }
}

void CScrollMsg::onItemClicked(int index)
{
    m_timer.stop();
    emit scrollMsgAbort(1);
    AppControl->OnSoftButtonClick(m_listButton.at(index).btnId,0);
}

void CScrollMsg::onItemLongClicked(int index)
{
    m_timer.stop();
    emit scrollMsgAbort(1);
    AppControl->OnSoftButtonClick(m_listButton.at(index).btnId,1);
}

void CScrollMsg::scrollMsgAbortSlots(int reason)
{
    //_D("smID=%d, reason=%d\n",smID,reason);
    AppControl->OnScrollMessageResponse(reason);
}


void CScrollMsg::showEvent(QShowEvent * e)
{
    for(int i = 0;i != 4;++i)
    {
        disconnect(m_btnSoft[i], SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));
    }

    if (AppControl)
    {
        m_listButton.clear();
        Json::Value m_jsonData = AppControl->getScrollableMsgJson()["params"];

        setTimeOut(m_jsonData["timeout"].asInt());

        if (m_jsonData.isMember("messageText"))
        {
            setMessage(m_jsonData["messageText"]["fieldText"].asString().data());
        }
        if (m_jsonData.isMember("softButtons"))
        {
            int size=m_jsonData["softButtons"].size();
            for (int i = 0; i < size; i++)
            {
                addSoftButton(m_jsonData["softButtons"][i]["softButtonID"].asInt(),
                        m_jsonData["softButtons"][i]["text"].asString().c_str(),m_jsonData["softButtons"][i]["isHighlighted"].asBool());
                connect(m_btnSoft[i], SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));
                connect(m_btnSoft[i], SIGNAL(clickedLong(int)), this, SLOT(onButtonClickedLongSlots(int)));
            }
        }
		UpdateLayout();
    }
}
