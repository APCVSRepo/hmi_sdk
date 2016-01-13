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
    m_editText = new QTextEdit;
    char* image[4]={":/images/softbutton_alert.png",
                   ":/images/softbutton_alert_left.png",
                   ":/images/softbutton_alert_right.png",
                   ":/images/softbutton_alert.png"};
    char* text[4]={"Soft1","Soft2","Soft3","Soft4"};
    for(int i=0;i<4;i++){
        m_btnSoft[i]=new CButton;
        m_btnSoft[i]->initParameter(ui_aler_width,ui_aler_height,image[i],image[i],"",text[i]);
        m_btnSoft[i]->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
        connect(m_btnSoft[i], SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));

        connect(m_btnSoft[i], SIGNAL(clickedLong(int)), this, SLOT(onButtonClickedLongSlots(int)));
    }



    QPalette pll = m_editText->palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    m_editText->setPalette(pll);
//    m_editText->setFixedSize(600,250);
    m_editText->setAttribute(Qt::WA_TranslucentBackground, true);
    //m_editText->setReadOnly(true); //设置不可编辑
    m_editText->setFrameShape(QFrame::NoFrame); //设置无边框
    m_editText->setStyleSheet(ScrollBar::cssString()+"border:1px;background-color:white;color:grey;font:36px \"Liberation Serif\";");

    m_editText->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QHBoxLayout *upLayout = new QHBoxLayout;
    upLayout->addStretch(90);
    //upLayout->addWidget(m_btn_up, 3);
    upLayout->addStretch(10);

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addStretch(90);
    //downLayout->addWidget(m_btn_down, 3);
    downLayout->addStretch(10);

    QHBoxLayout *midLayout = new QHBoxLayout;
    midLayout->addStretch(5);
    midLayout->addWidget(m_editText, 85);
    midLayout->addStretch(10);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_btnSoft[0]);
    bottomLayout->addWidget(m_btnSoft[1]);
    bottomLayout->addWidget(m_btnSoft[2]);
    bottomLayout->addWidget(m_btnSoft[3]);

    QVBoxLayout *mLayout = new QVBoxLayout(this);
    mLayout->addStretch(2);
    mLayout->addLayout(upLayout, 7);
//    mLayout->addWidget(m_editText, 66, Qt::AlignCenter);
    mLayout->addLayout(midLayout, 61);
    mLayout->addLayout(downLayout, 7);
    mLayout->addStretch(2);
    mLayout->addLayout(bottomLayout, 20);
    mLayout->addStretch(1);
    mLayout->setMargin(0);

//    m_labelFrame.setLayout(mLayout);
    this->setLayout(mLayout);

    // Test
    setMessage("hello world!");
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

void CScrollMsg::setBtnText(int btnIdx, QString text, bool highLight)
{
    switch (btnIdx)
    {
    case 0:
    {
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

void CScrollMsg::setBtnID(int btnIdx, int id)
{
    if(btnIdx<4)
       m_btnSoft[btnIdx]->setId(id);
}

void CScrollMsg::onSpaceClicedSlots()
{
    m_timer.stop();
    emit scrollMsgAbort(2);
}

void CScrollMsg::onButtonClickedSlots(int btID)
{
    m_timer.stop();
    emit scrollMsgAbort(1);
    if(btID != 0)
    {
        m_pList->getActiveApp()->OnSoftButtonClick(btID, 0);
    }
}

void CScrollMsg::onButtonClickedLongSlots(int btID)
{
    m_timer.stop();
    emit scrollMsgAbort(1);
    if(btID != 0)
    {
        m_pList->getActiveApp()->OnSoftButtonClick(btID, 1);
    }
}

void CScrollMsg::scrollMsgAbortSlots(int reason)
{
    //_D("smID=%d, reason=%d\n",smID,reason);
    m_pList->getActiveApp()->OnScrollMessageResponse(reason);
}


void CScrollMsg::showEvent(QShowEvent * e)
{
    if (m_pList->getActiveApp())
    {
        Json::Value m_jsonData = m_pList->getActiveApp()->getScrollableMsgJson()["params"];

        setTimeOut(m_jsonData["timeout"].asInt());
        setBtnText(0, "-", false);
        setBtnText(1, "-", false);
        setBtnText(2, "-", false);
        setBtnText(3, "-", false);

        if (m_jsonData.isMember("messageText"))
        {
            setMessage(m_jsonData["messageText"]["fieldText"].asString().data());
        }
        if (m_jsonData.isMember("softButtons"))
        {
            for (int i = 0; i < m_jsonData["softButtons"].size(); i++)
            {
                setBtnText(i, m_jsonData["softButtons"][i]["text"].asString().c_str(),m_jsonData["softButtons"][i]["isHighlighted"].asBool());
                setBtnID(i, m_jsonData["softButtons"][i]["softButtonID"].asInt());
            }
        }
    }
}
