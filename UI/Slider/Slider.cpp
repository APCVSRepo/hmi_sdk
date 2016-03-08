#include "Slider.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "UI/Config/Config.h"
#include "Common/AppBase.h"

Slider::Slider(AppListInterface * pList, QWidget *parent) :
  AppBase(pList, parent),m_iPos(0),m_bDynamic(false)
{
    InitLayout();
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timeoutSlots()));
    connect(this,SIGNAL(sliderClicked(int,int)),this,SLOT(sliderClickedSlots(int,int)));
    connect(this, SIGNAL(onSpaceCliced()), this, SLOT(onButtonCancelClicked()));
}

Slider::~Slider()
{
    delete m_labelText1;
    delete m_labelText2;
    delete m_labelText3;
    delete m_btnSoft1;
    delete m_btnSoft2;
    delete m_btnSoft3;
    delete m_btnSoft4;
}

void Slider::InitLayout()
{
    m_labelText1 = new QLabel;
    m_labelText2 = new QLabel;
    m_labelText3 = new QLabel;
    m_btnSoft1 = new CButton;
    m_btnSoft2 = new CButton;
    m_btnSoft3 = new CButton;
    m_btnSoft4 = new CButton;

    QHBoxLayout *btnLayout = new QHBoxLayout;
    QVBoxLayout *mLayout = new QVBoxLayout(this);
    btnLayout->addWidget(m_btnSoft1, 25);
    btnLayout->addWidget(m_btnSoft2, 25);
    btnLayout->addWidget(m_btnSoft3, 25);
    btnLayout->addWidget(m_btnSoft4, 25);

    mLayout->addWidget(m_labelText1, 24);
    mLayout->addWidget(m_labelText2, 24);
    mLayout->addWidget(m_labelText3, 24);
    mLayout->addLayout(btnLayout, 23);
    mLayout->addStretch(5);
    mLayout->setMargin(0);

    connect(m_btnSoft1, SIGNAL(clicked()), this, SLOT(onButtonSaveClicked()));
    connect(m_btnSoft2, SIGNAL(clicked()), this, SLOT(onMoveLeftSlot()));
    connect(m_btnSoft3, SIGNAL(clicked()), this, SLOT(onMoveRightSlot()));
    connect(m_btnSoft4, SIGNAL(clicked()), this, SLOT(onButtonCancelClicked()));

//    m_labelBackground.setGeometry((this->width() - iW) / 2, 0.85 * this->height() - iH, iW, iH);
//    m_labelFrame.setGeometry((this->width() - iW) / 2, 0.85 * this->height() - iH, iW, iH);

    m_labelText1->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:rgb(255,255,254)");
    m_labelText2->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:rgb(255,255,254)");
    m_labelText3->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:rgb(255,255,254)");
    m_labelText1->setAlignment(Qt::AlignCenter);
    m_labelText2->setAlignment(Qt::AlignCenter);
    m_labelText3->setAlignment(Qt::AlignCenter);

//    //test
//    QString tmp[5] = {"Hello", "Jack", "Moky", "Luckey", "Mono"};
//    setSliderStrings(tmp, 5, 1);
//    setSliderTitle("Test string.");
//    setPosition(3);

    m_btnSoft1->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft2->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft3->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft4->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");

    m_btnSoft1->initParameter(ui_aler_width, ui_aler_height, ":/images/softbutton_alert.png", ":/images/softbutton_alert.png");
    m_btnSoft2->initParameter(ui_aler_width, ui_aler_height, ":/images/softbutton_alert_left.png", ":/images/softbutton_alert_left.png", "", "");
    m_btnSoft2->setIconExtra(":/images/leftarrow.png");
    m_btnSoft3->initParameter(ui_aler_width, ui_aler_height, ":/images/softbutton_alert_right.png", ":/images/softbutton_alert_right.png", "", "");
    m_btnSoft3->setIconExtra(":/images/rightarrow.png");
    m_btnSoft4->initParameter(ui_aler_width, ui_aler_height, ":/images/softbutton_alert.png", ":/images/softbutton_alert.png");

//    m_labelFrame.setLayout(mLayout);
    //this->setLayout(mLayout);

    m_btnSoft1->setText("-");
    m_btnSoft4->setText("-");
}

void Slider::sliderClickedSlots( int code, int sliderPosition)
{
    if(AppControl)
    {
        AppControl->OnSliderResponse(code, sliderPosition);
    }
}

void Slider::onMoveLeftSlot()
{
    if (m_iPos > 0)
    {
        m_iPos--;
        updateScreen();
    }
}

void Slider::onMoveRightSlot()
{
    if (m_iPos < m_Strings.size() - 1)
    {
        m_iPos++;
        updateScreen();
    }
}

void Slider::setSliderTitle(QString text)
{
    SetEdlidedText(m_labelText1,text,width());
}

//void Slider::setSliderStrings(QString* pStr, int iCnt, int iPos)
void Slider::setSliderStrings(std::vector<std::string> vec_strSliter, int iPos)
{
    m_Strings.clear();
    for (int i = 0; i < vec_strSliter.size(); i++)
    {
        m_Strings.push_back(vec_strSliter.at(i).data());
    }

    m_iPos = iPos-1;

    updateScreen();
}

bool Slider::setPosition(int iPos)
{
    if (iPos < 0 || iPos >= m_Strings.size())
    {
        return false;
    }

    m_iPos = iPos;
    updateScreen();

    return true;
}

void Slider::updateScreen()
{
    m_timer.stop();
    m_timer.start();

    m_EditText.clear();
    for (int i = 0; i < m_Strings.size(); i++)
    {
        if (i == m_iPos)
        {
            m_EditText.push_back("|");
        }
        else
        {
            m_EditText.push_back(".");
        }
    }
    m_labelText2->setText("<" + m_EditText + ">");
    if(m_bDynamic)
    {
        SetEdlidedText(m_labelText3,m_Strings[m_iPos],width());
    }
    else
    {
        SetEdlidedText(m_labelText3,m_Strings[0],width());
    }
}

void Slider::setTimeOut(int duration)
{
    m_timer.setInterval(duration);
    m_timer.start();
}

void Slider::timeoutSlots()
{
    m_timer.stop();
    emit sliderClicked(SLIDER_TIMEOUT, m_iPos);
}

void Slider::onButtonSaveClicked()
{
    m_timer.stop();
    emit sliderClicked(SLIDER_OK, m_iPos);
}

void Slider::onButtonCancelClicked()
{
    m_timer.stop();
    emit sliderClicked(SLIDER_ABORTED, m_iPos);
}

void Slider::showEvent(QShowEvent * e)
{
    if (AppControl)
    {
        m_btnSoft1->setText("Save");
        m_btnSoft4->setText("Cancel");

        Json::Value m_jsonData = AppControl->getSlider();
//        this->setAppID(m_jsonData["params"]["appID"].asInt());
        this->setTimeOut(m_jsonData["params"]["timeout"].asInt());

        int numTicks = m_jsonData["params"]["numTicks"].asInt();
        int position = m_jsonData["params"]["position"].asInt();

        this->setSliderTitle(m_jsonData["params"]["sliderHeader"].asString().data());

        std::vector <std::string > vec_strSliter;
        vec_strSliter.clear();
        if (m_jsonData["params"].isMember("sliderFooter"))
        {
            for(int i = 0; i < m_jsonData["params"]["sliderFooter"].size(); i++)
            {
                vec_strSliter.push_back(m_jsonData["params"]["sliderFooter"][i].asString());
            }
        }
        if(vec_strSliter.size() == 1)
        {
            m_bDynamic = false;
        }
        else if(vec_strSliter.size() == numTicks)
        {
             m_bDynamic = true;
        }
        while(vec_strSliter.size() < numTicks)
        {
            vec_strSliter.push_back("-");
        }


        this->setSliderStrings(vec_strSliter,position);
    }
}
