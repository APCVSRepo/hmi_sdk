#include "ScrollMsg.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "UI/Config/Config.h"
#include <QTextLayout>
#include <QTextBlock>
extern Config g_config;



CScrollMsg::CScrollMsg(CPopBase *parent) : CPopBase(parent)
{
    m_labelBackground.setParent(this);
    m_editText = new QTextEdit;
    m_btn_up = new CButton;
    m_btn_down = new CButton;
    m_btnSoft1 = new CButton;
    m_btnSoft2 = new CButton;
    m_btnSoft3 = new CButton;
    m_btnSoft4 = new CButton;

    InitLayout();
    connect(this, SIGNAL(onSpaceCliced()), this, SLOT(onSpaceClicedSlots()));
}

CScrollMsg::~CScrollMsg()
{

}

void CScrollMsg::InitLayout()
{
    int iW = g_config.m_i_mainWindowW*6/7;
    int iH = g_config.m_i_mainWindowH*5/6;

    m_labelBackground.setParent(this);
    m_labelFrame.setParent(this);

    m_labelBackground.setGeometry((this->width() - iW) / 2, 0.9 * this->height() - iH, iW, iH);
    m_labelFrame.setGeometry((this->width() - iW) / 2, 0.9 * this->height() - iH, iW, iH);

//    m_btn_up->initParameter(30, 30, ":/images/uparrow.png", ":/images/uparrow.png", "", "");
//    m_btn_down->initParameter(30, 30, ":/images/downarrow.png", ":/images/downarrow.png", "", "");
    m_btn_up->setSize(24, 20);
    m_btn_down->setSize(24, 20);
    m_btn_up->setIconExtra(":/images/uparrow.png");
    m_btn_down->setIconExtra(":/images/downarrow.png");

    connect(m_btn_up,SIGNAL(clicked()),this,SLOT(upClickedSlots()));
    connect(m_btn_down,SIGNAL(clicked()),this,SLOT(downClickedSlots()));


    m_btnSoft1->initParameter(150, 70, ":/images/softbutton_alert.png", ":/images/softbutton_alert.png", "", "Soft1");
    m_btnSoft2->initParameter(150, 70, ":/images/softbutton_alert_left.png", ":/images/softbutton_alert_left.png", "", "Soft2");
    m_btnSoft3->initParameter(150, 70, ":/images/softbutton_alert_right.png", ":/images/softbutton_alert_right.png", "", "Soft3");
    m_btnSoft4->initParameter(150, 70, ":/images/softbutton_alert.png", ":/images/softbutton_alert.png", "", "Soft4");

    m_btnSoft1->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft2->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft3->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft4->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");

    connect(m_btnSoft1, SIGNAL(clicked(int)), this, SLOT(onButtonOneClickedSlots(int)));
    connect(m_btnSoft2, SIGNAL(clicked(int)), this, SLOT(onButtonTwoClickedSlots(int)));
    connect(m_btnSoft3, SIGNAL(clicked(int)), this, SLOT(onButtonThrClickedSlots(int)));
    connect(m_btnSoft4, SIGNAL(clicked(int)), this, SLOT(onButtonFouClickedSlots(int)));

    connect(m_btnSoft1, SIGNAL(clickedLong(int)), this, SLOT(onButtonOneClickedLongSlots(int)));
    connect(m_btnSoft2, SIGNAL(clickedLong(int)), this, SLOT(onButtonTwoClickedLongSlots(int)));
    connect(m_btnSoft3, SIGNAL(clickedLong(int)), this, SLOT(onButtonThrClickedLongSlots(int)));
    connect(m_btnSoft4, SIGNAL(clickedLong(int)), this, SLOT(onButtonFouClickedLongSlots(int)));


    QPalette pll = m_editText->palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    m_editText->setPalette(pll);
//    m_editText->setFixedSize(600,250);
    m_editText->setAttribute(Qt::WA_TranslucentBackground, true);
    //m_editText->setReadOnly(true); //设置不可编辑
    m_editText->setFrameShape(QFrame::NoFrame); //设置无边框
    m_editText->setStyleSheet("border:0px;color:white;font: 36px Liberation Serif;");
    m_editText->verticalScrollBar()->setStyleSheet(
                "QScrollBar{background:#3F4657; width: 20px;}"
                "QScrollBar::handle{background:#6C717D;}"
                "QScrollBar::handle:vertical {background: rgb(0, 0, 0);"
    );
    m_editText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QHBoxLayout *upLayout = new QHBoxLayout;
    upLayout->addStretch(90);
    upLayout->addWidget(m_btn_up, 3);
    upLayout->addStretch(10);

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addStretch(90);
    downLayout->addWidget(m_btn_down, 3);
    downLayout->addStretch(10);

    QHBoxLayout *midLayout = new QHBoxLayout;
    midLayout->addStretch(5);
    midLayout->addWidget(m_editText, 85);
    midLayout->addStretch(10);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_btnSoft1);
    bottomLayout->addWidget(m_btnSoft2);
    bottomLayout->addWidget(m_btnSoft3);
    bottomLayout->addWidget(m_btnSoft4);

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

    m_labelFrame.setLayout(mLayout);

    // Test
    setMessage("hello world!");
}

void CScrollMsg::upClickedSlots()
{
    m_editText->verticalScrollBar()->setValue(m_editText->verticalScrollBar()->value()-20);
}

void CScrollMsg::downClickedSlots()
{
    m_editText->verticalScrollBar()->setValue(m_editText->verticalScrollBar()->value()+20);
}

void CScrollMsg::setTimeOut(int duration)
{
    m_timer->start(duration);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutSlots()));
}

void CScrollMsg::timeoutSlots()
{
    m_timer->stop();
    emit scrollMsgAbort(m_i_scrollMsgID, 0);
    this->hide();
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
        m_btnSoft1->setText(text);
        if(highLight)
        {
            m_btnSoft1->setIconNormal(":/images/highlightsoftbutton_alert.png");
            m_btnSoft1->setIconPressed(":/images/highlightsoftbutton_alert.png");
        }
        else
        {
            m_btnSoft1->setIconNormal(":/images/softbutton_alert.png");
            m_btnSoft1->setIconPressed(":/images/softbutton_alert.png");
        }
    }
        break;
    case 1:
    {
        m_btnSoft2->setText(text);
        if(highLight)
        {
            m_btnSoft2->setIconNormal(":/images/highlightsoftbutton_alert_left.png");
            m_btnSoft2->setIconPressed(":/images/highlightsoftbutton_alert_left.png");
        }
        else
        {
            m_btnSoft2->setIconNormal(":/images/softbutton_alert_left.png");
            m_btnSoft2->setIconPressed(":/images/softbutton_alert_left.png");
        }
    }
        break;
    case 2:
    {
        m_btnSoft3->setText(text);
        if(highLight)
        {
            m_btnSoft3->setIconNormal(":/images/highlightsoftbutton_alert_right.png");
            m_btnSoft3->setIconPressed(":/images/highlightsoftbutton_alert_right.png");
        }
        else
        {
            m_btnSoft3->setIconNormal(":/images/softbutton_alert_right.png");
            m_btnSoft3->setIconPressed(":/images/softbutton_alert_right.png");
        }
    }
        break;
    case 3:
    {
        m_btnSoft4->setText(text);
        if(highLight)
        {
            m_btnSoft1->setIconNormal(":/images/highlightsoftbutton_alert.png");
            m_btnSoft1->setIconPressed(":/images/highlightsoftbutton_alert.png");
        }
        else
        {
            m_btnSoft1->setIconNormal(":/images/softbutton_alert.png");
            m_btnSoft1->setIconPressed(":/images/softbutton_alert.png");
        }
    }
        break;
    }
}

void CScrollMsg::setBtnID(int btnIdx, int id)
{
    switch (btnIdx)
    {
    case 0:
        m_btnSoft1->setId(id);
        break;
    case 1:
        m_btnSoft2->setId(id);
        break;
    case 2:
        m_btnSoft3->setId(id);
        break;
    case 3:
        m_btnSoft4->setId(id);
        break;
    }
}

void CScrollMsg::onSpaceClicedSlots()
{
    m_timer->stop();
    emit scrollMsgAbort(m_i_scrollMsgID, 2);
    this->hide();
}

void CScrollMsg::onButtonOneClickedSlots(int btID)
{
    m_timer->stop();
    emit scrollMsgAbort(m_i_scrollMsgID, 1);
    this->hide();
    if(btID != 0)
    {
        emit softButtonClicked(btID, 0);
    }
}

void CScrollMsg::onButtonTwoClickedSlots(int btID)
{
    m_timer->stop();
    emit scrollMsgAbort(m_i_scrollMsgID, 1);
    this->hide();
    if(btID != 0)
    {
        emit softButtonClicked(btID, 0);
    }
}

void CScrollMsg::onButtonThrClickedSlots(int btID)
{
    m_timer->stop();
    emit scrollMsgAbort(m_i_scrollMsgID, 1);
    this->hide();
    if(btID != 0)
    {
        emit softButtonClicked(btID, 0);
    }
}

void CScrollMsg::onButtonFouClickedSlots(int btID)
{
    m_timer->stop();
    emit scrollMsgAbort(m_i_scrollMsgID, 1);
    this->hide();
    if(btID != 0)
    {
        emit softButtonClicked(btID, 0);
    }
}

void CScrollMsg::onButtonOneClickedLongSlots(int btID)
{
    m_timer->stop();
    emit scrollMsgAbort(m_i_scrollMsgID, 1);
    this->hide();
    if(btID != 0)
    {
        emit softButtonClicked(btID, 1);
    }
}

void CScrollMsg::onButtonTwoClickedLongSlots(int btID)
{
    m_timer->stop();
    emit scrollMsgAbort(m_i_scrollMsgID, 1);
    this->hide();
    if(btID != 0)
    {
        emit softButtonClicked(btID, 1);
    }
}

void CScrollMsg::onButtonThrClickedLongSlots(int btID)
{
    m_timer->stop();
    emit scrollMsgAbort(m_i_scrollMsgID, 1);
    this->hide();
    if(btID != 0)
    {
        emit softButtonClicked(btID, 1);
    }
}

void CScrollMsg::onButtonFouClickedLongSlots(int btID)
{
    m_timer->stop();
    emit scrollMsgAbort(m_i_scrollMsgID, 1);
    this->hide();
    if(btID != 0)
    {
        emit softButtonClicked(btID, 1);
    }
}

void CScrollMsg::execShow(AppDataInterface* pAppInterface)
{
    if (pAppInterface)
    {
        m_jsonData = pAppInterface->getScrollableMsgJson()["params"];

        this->setTimeOut(m_jsonData["timeout"].asInt());
        this->setBtnText(0, "-", false);
        this->setBtnText(1, "-", false);
        this->setBtnText(2, "-", false);
        this->setBtnText(3, "-", false);

        if (m_jsonData.isMember("messageText"))
        {
            this->setMessage(m_jsonData["messageText"]["fieldText"].asString().data());
        }
        if (m_jsonData.isMember("softButtons"))
        {
            for (int i = 0; i < m_jsonData["softButtons"].size(); i++)
            {
                this->setBtnText(i, m_jsonData["softButtons"][i]["text"].asString().c_str(),m_jsonData["softButtons"][i]["isHighlighted"].asBool());
                this->setBtnID(i, m_jsonData["softButtons"][i]["softButtonID"].asInt());
            }
        }
    }
    this->show();
}
