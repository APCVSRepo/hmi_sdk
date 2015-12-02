#include "AlertUI.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "UI/Config/Config.h"


CAlertUI::CAlertUI(QWidget *parent) : CPopBase(parent), mLayout(NULL)
{
    InitLayout();
    connect(this, SIGNAL(onSpaceCliced()), this, SLOT(onSpaceClicedSlots()));
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

    connect(m_btnSoft1, SIGNAL(clicked(int)), this, SLOT(onButtonOneClickedSlots(int)));
    connect(m_btnSoft2, SIGNAL(clicked(int)), this, SLOT(onButtonTwoClickedSlots(int)));
    connect(m_btnSoft3, SIGNAL(clicked(int)), this, SLOT(onButtonThrClickedSlots(int)));
    connect(m_btnSoft4, SIGNAL(clicked(int)), this, SLOT(onButtonFouClickedSlots(int)));

    connect(m_btnSoft1, SIGNAL(clickedLong(int)), this, SLOT(onButtonOneClickedLongSlots(int)));
    connect(m_btnSoft2, SIGNAL(clickedLong(int)), this, SLOT(onButtonTwoClickedLongSlots(int)));
    connect(m_btnSoft3, SIGNAL(clickedLong(int)), this, SLOT(onButtonThrClickedLongSlots(int)));
    connect(m_btnSoft4, SIGNAL(clickedLong(int)), this, SLOT(onButtonFouClickedLongSlots(int)));

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

    // test
    m_labelText1->setText("Alert text 1");
    m_labelText2->setText("Alert text 2");
    m_labelText3->setText("Alert text 3");

    updateLayout();

}

void CAlertUI::updateLayout()
{
    int iW = ui_app_width;
    int iH = 0;

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
        iH = m_labelText1->height() + m_labelText2->height() + m_labelText3->height();
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
        iH = m_labelText1->height() + m_labelText2->height();
    }
    else if (!m_labelText1->text().isEmpty())
    {
        m_labelText1->setFixedHeight(ui_aler_height);
        mLayout->addWidget(m_labelText1, Qt::AlignTop);
        iH = m_labelText1->height();
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

    iH += m_btnSoft1->height() + 40;
//    m_labelBackground.setGeometry((width() - iW) / 2, 0.85 * height() - iH, iW, iH);
//    m_labelFrame.setGeometry((width() - iW) / 2, 0.85 * height() - iH, iW, iH);

//    m_labelFrame.setLayout(mLayout);
    this->setLayout(mLayout);
}

void CAlertUI::setTimeOut(int duration)
{
    m_timer->start(duration);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutSlots()));
}

void CAlertUI::setAlertText(int textIdx, QString text)
{
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
    m_timer->stop();
    alertAbortSlots(m_i_alertID, 0);
    goBack();
}

void CAlertUI::onSpaceClicedSlots()
{
    m_timer->stop();
    alertAbortSlots(m_i_alertID, 2);
    goBack();
}

void CAlertUI::onButtonOneClickedSlots(int btID)
{
    m_timer->stop();
    alertAbortSlots(m_i_alertID, 1);
    goBack();
    if(btID != 0)
    {
        DataManager::DataInterface()->OnSoftButtonClick(btID,0);
    }
}

void CAlertUI::onButtonTwoClickedSlots(int btID)
{
    m_timer->stop();
    alertAbortSlots(m_i_alertID, 1);
    goBack();
    if(btID != 0)
    {
        DataManager::DataInterface()->OnSoftButtonClick(btID, 0);
    }
}

void CAlertUI::onButtonThrClickedSlots(int btID)
{
    m_timer->stop();
    alertAbortSlots(m_i_alertID, 1);
    goBack();
    if(btID != 0)
    {
        DataManager::DataInterface()->OnSoftButtonClick(btID, 0);
    }
}

void CAlertUI::onButtonFouClickedSlots(int btID)
{
    m_timer->stop();
    alertAbortSlots(m_i_alertID, 1);
    goBack();
    if(btID != 0)
    {
        DataManager::DataInterface()->OnSoftButtonClick(btID, 0);
    }
}

void CAlertUI::onButtonOneClickedLongSlots(int btID)
{
    m_timer->stop();
    alertAbortSlots(m_i_alertID, 1);
    goBack();
    if(btID != 0)
    {
        DataManager::DataInterface()->OnSoftButtonClick(btID, 1);
    }
}

void CAlertUI::onButtonTwoClickedLongSlots(int btID)
{
    m_timer->stop();
    alertAbortSlots(m_i_alertID, 1);
    goBack();
    if(btID != 0)
    {
        DataManager::DataInterface()->OnSoftButtonClick(btID, 1);
    }
}

void CAlertUI::onButtonThrClickedLongSlots(int btID)
{
    m_timer->stop();
    alertAbortSlots(m_i_alertID, 1);
    goBack();
    if(btID != 0)
    {
        DataManager::DataInterface()->OnSoftButtonClick(btID, 1);
    }
}

void CAlertUI::onButtonFouClickedLongSlots(int btID)
{
    m_timer->stop();
    alertAbortSlots(m_i_alertID, 1);
    goBack();
    if(btID != 0)
    {
        DataManager::DataInterface()->OnSoftButtonClick(btID, 1);
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
void CAlertUI::alertAbortSlots(int alertID, int reason)
{
    //_D("alertID=%d, reason=%d\n",alertID,reason);
    DataManager::DataInterface()->OnAlertResponse(alertID,reason);
}

void CAlertUI::execShow()
{
    Json::Value pObj;
    int itemCnt = 0;

    if (DataManager::DataInterface())
    {
        pObj = DataManager::DataInterface()->getAlertJson();

        m_i_alertID = pObj["id"].asInt();

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
                    setBtnText(i, pObj["params"]["softButtons"][i]["text"].asString().c_str(),pObj["params"]["softButtons"][i]["isHighlighted"].asBool());
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
    this->show();
}

void CAlertUI::testShow()
{
    m_i_alertID = 1;
    setAlertText(0, "alert1");
    setAlertText(1, "alert2");
    setAlertText(2, "alert3");

    setBtnText(0, "test1",true);
    setBtnText(1, "test2",true);
    setBtnText(2, "test3",true);
    setBtnText(3, "test4",false);
    setTimeOut(20000);

    updateLayout();
    this->show();
}
