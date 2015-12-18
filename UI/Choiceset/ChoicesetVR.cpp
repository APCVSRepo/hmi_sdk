#include "ChoicesetVR.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "UI/Config/Config.h"
#include "Common/PopBase.h"
CChoicesetVR::CChoicesetVR(AppListInterface * pList, QWidget *parent) : CPopBase(pList, parent)
{
    InitLayout();
    connect(this, SIGNAL(onSpaceCliced()), this, SLOT(hide()));
    connect(this,SIGNAL(VRmenuClicked(int,int,int)),this,SLOT(VRmenuClickedSlots(int,int,int)));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutSlots()));
}

CChoicesetVR::~CChoicesetVR()
{

}

void CChoicesetVR::InitLayout()
{
    m_labelVRIcon = new QLabel;
    m_labelText = new QLabel;
    m_labelSet1 = new CButton;
    m_labelSet2 = new CButton;
    m_labelSet3 = new CButton;
    m_labelSet4 = new CButton;

//    m_labelBackground.setGeometry((this->width() - iW) / 2, 0.85 * this->height() - iH, iW, iH);
//    m_labelFrame.setGeometry((this->width() - iW) / 2, 0.85 * this->height() - iH, iW, iH);

    m_labelText->setStyleSheet("border: 2px solid white;color:white;font: 42px \"Liberation Serif\";border-radius:8px;border-top:2px solid black;background-color: #121122;"); //qproperty-alignment:AlignLeft;");
    m_labelText->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_labelVRIcon->setStyleSheet("color:white;font: 20px \"Liberation Serif\";border-image: url(:/images/ear.png);");

    m_labelSet1->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:white;");
    m_labelSet2->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:white;");
    m_labelSet3->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:white;");
    m_labelSet4->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:white;");

    QVBoxLayout *textLayout = new QVBoxLayout;
    QVBoxLayout *iconLayout = new QVBoxLayout;
    QHBoxLayout *mLayout = new QHBoxLayout;

    iconLayout->addStretch(2);
    iconLayout->addWidget(m_labelVRIcon, 15);
    iconLayout->addStretch(83);

    textLayout->addWidget(m_labelText, 20);
    textLayout->addWidget(m_labelSet1, 20);
    textLayout->addWidget(m_labelSet2, 20);
    textLayout->addWidget(m_labelSet3, 20);
    textLayout->addWidget(m_labelSet4, 20);

    mLayout->addStretch(2);
    mLayout->addLayout(iconLayout, 8);
    mLayout->addLayout(textLayout, 80);
    mLayout->addStretch(10);
    mLayout->setMargin(8);

//    m_labelFrame.setLayout(mLayout);
    this->setLayout(mLayout);

    connect(m_labelSet1,SIGNAL(clicked(int)),SLOT(label1ClickedSlots(int)));
    connect(m_labelSet2,SIGNAL(clicked(int)),SLOT(label2ClickedSlots(int)));
    connect(m_labelSet3,SIGNAL(clicked(int)),SLOT(label3ClickedSlots(int)));
    connect(m_labelSet4,SIGNAL(clicked(int)),SLOT(label4ClickedSlots(int)));

    // test
    m_labelText->setText("Search commands.");
    m_labelSet1->setText("VR command1");
    m_labelSet2->setText("VR command2");
    m_labelSet3->setText("VR command3");
    m_labelSet4->setText("VR command4");
}

void CChoicesetVR::setChoicesetVRText(int textIdx, QString text)
{
    switch (textIdx)
    {
    case 0:
        m_labelText->setText(text);
        break;
    case 1:
        m_labelSet1->setText(text);
        break;
    case 2:
        m_labelSet2->setText(text);
        break;
    case 3:
        m_labelSet3->setText(text);
        break;
    case 4:
        m_labelSet4->setText(text);
        break;
    default:
        break;
    }
}

void CChoicesetVR::setChoicesetVRID(int btnIdx, int id)
{
    switch (btnIdx)
    {
    case 0:
        m_labelSet1->setId(id);
        m_i_defaultID = id;
        break;
    case 1:
        m_labelSet2->setId(id);
        break;
    case 2:
        m_labelSet3->setId(id);
        break;
    case 3:
        m_labelSet4->setId(id);
        break;
    default:
        break;
    }
}

void CChoicesetVR::setInteractionID(int id)
{
    m_i_interactionID = id;
}

void CChoicesetVR::setTimeOut(int duration)
{
    m_timer->setInterval(duration);
    m_timer->start();

}

void CChoicesetVR::timeoutSlots()
{
    m_timer->stop();

    emit VRmenuClicked(PERFORMINTERACTION_TIMEOUT, m_i_interactionID, m_i_defaultID);
    goBack();
}

void CChoicesetVR::label1ClickedSlots(int id)
{
    m_timer->stop();

    emit VRmenuClicked(PERFORMINTERACTION_CHOICE, m_i_interactionID, id);
    goBack();
}

void CChoicesetVR::label2ClickedSlots(int id)
{
    m_timer->stop();

    emit VRmenuClicked(PERFORMINTERACTION_CHOICE, m_i_interactionID, id);
    goBack();
}

void CChoicesetVR::label3ClickedSlots(int id)
{
    m_timer->stop();

    emit VRmenuClicked(PERFORMINTERACTION_CHOICE, m_i_interactionID, id);
    goBack();
}

void CChoicesetVR::label4ClickedSlots(int id)
{
    m_timer->stop();

    emit VRmenuClicked(PERFORMINTERACTION_CHOICE, m_i_interactionID, id);
    goBack();
}

void CChoicesetVR::VRmenuClickedSlots(int code, int performInteractionID, int choiceID)
{
    //_D("code=%d:%d:%d\n",code,performInteractionID,choiceID);
    m_pList->getActiveApp()->OnPerformInteraction(code, performInteractionID, choiceID);
    showCurUI(ID_SHOW);
}

void CChoicesetVR::execShow()
{
    if (m_pList->getActiveApp())
    {
        m_jsonData = m_pList->getActiveApp()->getInteractionJson();
        setTimeOut(m_jsonData["params"]["timeout"].asInt());

        for(int i = 0; i < m_jsonData["params"]["vrHelp"].size(); i++)
        {
            setChoicesetVRText(i+1,m_jsonData["params"]["vrHelp"][i]["text"].asString().data());
        }

        if(m_jsonData["params"].isMember("choiceSet"))
        {
            //BOTH
            for(int i = 0; i < m_jsonData["params"]["choiceSet"].size(); i++)
            {
//                this->addNewMenu(m_jsonData["params"]["choiceSet"][i]["choiceID"].asInt(),m_jsonData["params"]["choiceSet"][i]["menuName"].asString());
            }
        }
    }
    show();
}
