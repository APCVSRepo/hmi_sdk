#include "AlertUI.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "UI/Config/Config.h"


CAlertUI::CAlertUI(AppListInterface * pList, QWidget *parent) : AppBase(pList, parent), mLayout(NULL)
{
    InitLayout();
    connect(this, SIGNAL(onSpaceCliced()), this, SLOT(onSpaceClicedSlots()));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timeoutSlots()));
    //connect(this,SIGNAL(alertAbort(int,int)),this,SLOT(alertAbortSlots(int,int)));
    //connect(this,SIGNAL(softButtonClicked(int,int)),this,SLOT(softButtonClickedSlots(int,int)));
}

CAlertUI::~CAlertUI()
{

}

void CAlertUI::InitLayout()
{
//    m_labelBackground.setParent(this);
//    m_labelFrame.setParent(this);
    m_labelText1 = new QLabel;
    m_labelText2 = new QLabel;
    m_labelText3 = new QLabel;
    m_btnSoft1 = new CButton;
    m_btnSoft2 = new CButton;
    m_btnSoft3 = new CButton;
    m_btnSoft4 = new CButton;

    connect(m_btnSoft1, SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));
    connect(m_btnSoft2, SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));
    connect(m_btnSoft3, SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));
    connect(m_btnSoft4, SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));

    connect(m_btnSoft1, SIGNAL(clickedLong(int)), this, SLOT(onButtonClickedLongSlots(int)));
    connect(m_btnSoft2, SIGNAL(clickedLong(int)), this, SLOT(onButtonClickedLongSlots(int)));
    connect(m_btnSoft3, SIGNAL(clickedLong(int)), this, SLOT(onButtonClickedLongSlots(int)));
    connect(m_btnSoft4, SIGNAL(clickedLong(int)), this, SLOT(onButtonClickedLongSlots(int)));

    m_labelText1->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:rgb(255,255,254)");
    m_labelText2->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:rgb(255,255,254)");
    m_labelText3->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:rgb(255,255,254)");
    m_labelText1->setAlignment(Qt::AlignCenter);
    m_labelText2->setAlignment(Qt::AlignCenter);
    m_labelText3->setAlignment(Qt::AlignCenter);

    m_btnSoft1->initParameter(ui_aler_width,ui_aler_height, ":/images/softbutton_alert.png", ":/images/softbutton_alert.png", "", "Soft1");
    m_btnSoft2->initParameter(ui_aler_width,ui_aler_height, ":/images/softbutton_alert_left.png", ":/images/softbutton_alert_left.png", "", "Soft2");
    m_btnSoft3->initParameter(ui_aler_width,ui_aler_height, ":/images/softbutton_alert_right.png", ":/images/softbutton_alert_right.png", "", "Soft3");
    m_btnSoft4->initParameter(ui_aler_width,ui_aler_height, ":/images/softbutton_alert.png", ":/images/softbutton_alert.png", "", "Soft4");

    m_btnSoft1->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft2->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft3->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft4->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");

    updateLayout();
}

void CAlertUI::updateLayout()
{
    if (mLayout)
    {
        delete mLayout;
    }
    mLayout = new QVBoxLayout;

    if (!m_labelText3->text().isEmpty())
    {
        if (m_labelText1->text().isEmpty())
        {
            m_labelText1->setText(" ");
        }
        if (m_labelText2->text().isEmpty())
        {
            m_labelText2->setText(" ");
        }
//        m_labelText1->adjustSize();
//        m_labelText2->adjustSize();
//        m_labelText3->adjustSize();
        m_labelText1->setFixedHeight(ui_aler_height);
        m_labelText2->setFixedHeight(ui_aler_height);
        m_labelText3->setFixedHeight(ui_aler_height);
        mLayout->addWidget(m_labelText1, Qt::AlignTop);
        mLayout->addWidget(m_labelText2, Qt::AlignTop);
        mLayout->addWidget(m_labelText3, Qt::AlignTop);
    }
    else if (!m_labelText2->text().isEmpty())
    {
        if (m_labelText1->text().isEmpty())
        {
            m_labelText1->setText(" ");
        }
        m_labelText1->setFixedHeight(ui_aler_height);
        m_labelText2->setFixedHeight(ui_aler_height);
        mLayout->addWidget(m_labelText1, Qt::AlignTop);
        mLayout->addWidget(m_labelText2, Qt::AlignTop);
    }
    else if (!m_labelText1->text().isEmpty())
    {
        m_labelText1->setFixedHeight(ui_aler_height);
        mLayout->addWidget(m_labelText1, Qt::AlignTop);
    }
    else
    {

    }

    QHBoxLayout *btnLayout = new QHBoxLayout;
    m_btnSoft1->adjustSize();
    m_btnSoft2->adjustSize();
    m_btnSoft3->adjustSize();
    m_btnSoft4->adjustSize();
    mLayout->addSpacing(0);
    btnLayout->addWidget(m_btnSoft1);
    btnLayout->addWidget(m_btnSoft2);
    btnLayout->addWidget(m_btnSoft3);
    btnLayout->addWidget(m_btnSoft4);
    mLayout->addLayout(btnLayout);
    mLayout->addSpacing(0);
    mLayout->setMargin(0);

    this->setLayout(mLayout);
}

void CAlertUI::setTimeOut(int duration)
{
    m_timer.start(duration);

   // QTimer::singleShot(duration,this,SLOT(timeoutSlots()));
}

void CAlertUI::setAlertText(int textIdx, QString text)
{
    std::string strTemp("");
    if(text.length() > 22)
    {
        text = text.left(20);
        text += "...";
    }
    switch (textIdx)
    {
    case 0:
        m_labelText1->setText(text);
        break;
    case 1:
        m_labelText2->setText(text);
        break;
    case 2:
        m_labelText3->setText(text);
        break;
    }
}

void CAlertUI::setBtnText(int btnIdx, QString text, bool highLight)
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
            m_btnSoft4->setIconNormal(":/images/highlightsoftbutton_alert.png");
            m_btnSoft4->setIconPressed(":/images/highlightsoftbutton_alert.png");
        }
        else
        {
            m_btnSoft4->setIconNormal(":/images/softbutton_alert.png");
            m_btnSoft4->setIconPressed(":/images/softbutton_alert.png");
        }
    }
        break;
    }
}

void CAlertUI::timeoutSlots()
{
    m_timer.stop();
    alertAbortSlots(0);
}

void CAlertUI::onSpaceClicedSlots()
{
    m_timer.stop();
    alertAbortSlots(2);
}

void CAlertUI::onButtonClickedSlots(int btID)
{
    m_timer.stop();
    alertAbortSlots(1);
    if(btID != 0)
    {
        m_pList->getActiveApp()->OnSoftButtonClick(btID, 0);
    }
}

void CAlertUI::onButtonClickedLongSlots(int btID)
{
    m_timer.stop();
    alertAbortSlots(1);
    if(btID != 0)
    {
        m_pList->getActiveApp()->OnSoftButtonClick(btID, 1);
    }
}

//{
//   "id" : 65,
//   "jsonrpc" : "2.0",
//   "method" : "UI.Alert",
//   "params" : {
//      "alertStrings" : [
//         {
//            "fieldName" : "alertText1",
//            "fieldText" : "alertText1"
//         },
//         {
//            "fieldName" : "alertText2",
//            "fieldText" : "alertText2"
//         },
//         {
//            "fieldName" : "alertText3",
//            "fieldText" : "alertText3"
//         }
//      ],
//      "alertType" : "BOTH",
//      "appID" : 846930886,
//      "duration" : 5000,
//      "softButtons" : [
//         {
//            "isHighlighted" : false,
//            "softButtonID" : 1,
//            "systemAction" : "DEFAULT_ACTION",
//            "text" : "Softbutton1",
//            "type" : "TEXT"
//         },
//         {
//            "isHighlighted" : false,
//            "softButtonID" : 2,
//            "systemAction" : "DEFAULT_ACTION",
//            "text" : "Softbutton2",
//            "type" : "TEXT"
//         },
//         {
//            "isHighlighted" : false,
//            "softButtonID" : 3,
//            "systemAction" : "DEFAULT_ACTION",
//            "text" : "Softbutton3",
//            "type" : "TEXT"
//         },
//         {
//            "isHighlighted" : false,
//            "softButtonID" : 4,
//            "systemAction" : "DEFAULT_ACTION",
//            "text" : "Softbutton4",
//            "type" : "TEXT"
//         }
//      ]
//   }
//}
void CAlertUI::alertAbortSlots(int reason)
{
    //_D("alertID=%d, reason=%d\n",alertID,reason);
    m_pList->getActiveApp()->OnAlertResponse(reason);
}

void CAlertUI::showEvent(QShowEvent * e)
{
    Json::Value pObj;
    int itemCnt = 0;
    std::string strTemp("");

    if (m_pList->getActiveApp())
    {
        pObj = m_pList->getActiveApp()->getAlertJson();

        setAlertText(0,"");
        setAlertText(1,"");
        setAlertText(2,"");
        setBtnText(0, "-", false);
        setBtnText(1, "-", false);
        setBtnText(2, "-", false);
        setBtnText(3, "-", false);

        if (pObj["params"].isMember("alertStrings"))
        {
            itemCnt = pObj["params"]["alertStrings"].size();
            for(int i = 0; i < itemCnt; i++)
            {
                if (i < 3)
                {
                    setAlertText(i, pObj["params"]["alertStrings"][i]["fieldText"].asString().c_str());
                }
                else
                {
                    break;
                }
            }
        }

        if (pObj["params"].isMember("softButtons"))
        {
            setTimeOut(pObj["params"]["duration"].asInt()+20000);
            itemCnt = pObj["params"]["softButtons"].size();
            for (int i = 0; i < itemCnt; i++)
            {
                if (i < 4)
                {
                    // 软按键显示文字最多5个字符[20160104 wsw]
                    strTemp = pObj["params"]["softButtons"][i]["text"].asString();
                    if(strTemp.length() > 5)
                    {
                        strTemp = strTemp.substr(0,5);
                    }
                    setBtnText(i, strTemp.c_str(),pObj["params"]["softButtons"][i]["isHighlighted"].asBool());
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            setTimeOut(pObj["params"]["duration"].asInt());
        }
    }

    updateLayout();
}
