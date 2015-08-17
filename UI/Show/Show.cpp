#include "Show.h"
#include "UI/Command/Command.h"

Show::Show(QWidget *parent) : AppBase(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏

    connect(this,SIGNAL(moveBack()),this,SLOT(moveBackSlots()));

    m_btn_one = new CButton;
    m_btn_two = new CButton;
    m_btn_thr = new CButton;
    m_btn_fou = new CButton;

    initLayout();
    m_i_totalNum = 1;
    m_i_currentNo = 1;

    m_timer_mediaClock = new QTimer;
    m_timer_mediaClock->setInterval(1000);
    connect(m_timer_mediaClock,SIGNAL(timeout()),this,SLOT(mediaClockSlots()));
}

Show::~Show()
{
    delete m_btn_one;
    delete m_btn_two;
    delete m_btn_thr;
    delete m_btn_fou;
}

void Show::initLayout()
{

    QHBoxLayout *hhLayout = new QHBoxLayout;
    hhLayout->addWidget(&m_lab_mediaTrack,2,Qt::AlignLeft|Qt::AlignBottom);
    hhLayout->addStretch(1);
    hhLayout->addWidget(&m_lab_mediaClock,1,Qt::AlignRight|Qt::AlignBottom);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(&m_listWidget,60,Qt::AlignCenter);
    vLayout->addLayout(hhLayout,40);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(&m_lab_icon,22,Qt::AlignLeft);
    hLayout->addLayout(vLayout,68);
    hLayout->addWidget(&m_scrollBar,10);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_btn_one,1,Qt::AlignCenter);
    btnLayout->addWidget(m_btn_two,1,Qt::AlignCenter);
    btnLayout->addWidget(m_btn_thr,1,Qt::AlignCenter);
    btnLayout->addWidget(m_btn_fou,1,Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(hLayout,3);
    mainLayout->addLayout(btnLayout,1);

    QHBoxLayout *midLayout = new QHBoxLayout;
//    midLayout->addStretch(12);
    midLayout->addLayout(menuLayout,12);
    midLayout->addLayout(mainLayout,65);
    midLayout->addStretch(3);

    QVBoxLayout *mLayout = new QVBoxLayout(this);
    mLayout->addWidget(&m_lab_title,1, Qt::AlignCenter);
    mLayout->addLayout(midLayout, 4);
    mLayout->addWidget(&m_lab_time,1, Qt::AlignRight);

    mLayout->setMargin(0); //边框无缝
    mLayout->setSpacing(0);


    QPixmap pixmap(":/images/sync.png");
    QPixmap fitpixmap=pixmap.scaled(120,120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_lab_icon.setPixmap(fitpixmap);

    m_btn_one->initParameter(160,70,":/images/softbutton_mfd.png",":/images/softbutton_mfd.png");
    m_btn_two->initParameter(160,70,":/images/softbutton_mfd_left.png",":/images/softbutton_mfd_left.png");
    m_btn_thr->initParameter(160,70,":/images/softbutton_mfd_right.png",":/images/softbutton_mfd_right.png");
    m_btn_fou->initParameter(160,70,":/images/softbutton_mfd.png",":/images/softbutton_mfd.png");
    m_btn_one->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btn_two->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btn_thr->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btn_fou->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");



    connect(&m_listWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListSelect(QModelIndex)));

#ifndef ANDROID
    QPalette pll = m_listWidget.palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
    m_listWidget.setPalette(pll);
#endif
    m_listWidget.setFixedSize(400,160);
    m_listWidget.setFrameShape(QFrame::NoFrame); //设置无边框
    m_listWidget.setFocusPolicy(Qt::NoFocus); //去除选中虚线框
    m_listWidget.setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
#ifdef ANDROID
    m_listWidget.setStyleSheet("background-color:transparent");
#else
    m_listWidget.setStyleSheet("QListWidget:item:hover{border: 0px;}"); //鼠标移上去不响应突出
#endif
    m_listWidget.verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 0px;}");

    addListItem();
    addListItem();
    addListItem();
    addListItem();

    m_lab_mediaTrack.setStyleSheet("font: 75 36pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");
    m_lab_mediaClock.setStyleSheet("font: 75 36pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");

    m_scrollBar.init(1, 220);
    m_scrollBar.flushScroll(1,2);
    m_scrollBar.hide();
    connect(&m_scrollBar,SIGNAL(upClicked()),this,SLOT(upArrowSlots()));
    connect(&m_scrollBar,SIGNAL(downClicked()),this,SLOT(downArrowSlots()));





    m_btn_one->setText("-");
    m_btn_two->setText("-");
    m_btn_thr->setText("-");
    m_btn_fou->setText("More...");
    m_btn_fou->setIconNormal(":/images/softbutton_mfd.png");
    m_btn_fou->setIconPressed(":/images/softbutton_mfd.png");

    connect(m_btn_one,SIGNAL(clicked(int)),this,SLOT(btnOneClickedSlots(int)));
    connect(m_btn_two,SIGNAL(clicked(int)),this,SLOT(btnTwoClickedSlots(int)));
    connect(m_btn_thr,SIGNAL(clicked(int)),this,SLOT(btnThrClickedSlots(int)));
    connect(m_btn_fou,SIGNAL(clicked()),this,SLOT(btnFourClickedSlots()));

    connect(m_btn_one,SIGNAL(clickedLong(int)),this,SLOT(btnOneClickedLongSlots(int)));
    connect(m_btn_two,SIGNAL(clickedLong(int)),this,SLOT(btnTwoClickedLongSlots(int)));
    connect(m_btn_thr,SIGNAL(clickedLong(int)),this,SLOT(btnThrClickedLongSlots(int)));


   // m_btn_one->changeToPressed();



    setMainField1(true,"");
    setMainField2(true,"");
    setMainField3(true,"");
    setMainField4(false,"");
    setMediaTrack(true,"");
    setMediaClock(true,"");
//    setSoftButtons(std::vector<SSoftButton> vec_softButtons);
    m_vec_softButtons.clear();

}

void Show::addListItem()
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(QSize(400-10,80));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsDragEnabled);//不响应突出

    QLabel *label = new QLabel;
    m_listWidget.addItem(item);
    m_listWidget.setItemWidget(item,label);

    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font: 75 36pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");

    m_vec_listItem.append(item);
    m_vec_listLabel.append(label);
}



void Show::setMainField1(bool isShow, QString text)
{
    if(isShow)
        m_vec_listLabel.at(0)->setText(text);
    else
        m_vec_listLabel.at(0)->clear();
}
void Show::setMainField2(bool isShow, QString text)
{
    if(isShow)
        m_vec_listLabel.at(1)->setText(text);
    else
        m_vec_listLabel.at(1)->clear();
}
void Show::setMainField3(bool isShow, QString text)
{
    if(isShow)
    {
        m_vec_listLabel.at(2)->setText(text);
        m_scrollBar.show();
    }
    else
    {
        m_vec_listLabel.at(2)->clear();
        if(0 == m_vec_listLabel.at(3)->text().size())
            m_scrollBar.hide();
    }
}
void Show::setMainField4(bool isShow, QString text)
{
    if(isShow)
    {
        m_vec_listLabel.at(3)->setText(text);
        m_scrollBar.show();
    }
    else
    {
        m_vec_listLabel.at(3)->clear();
        if(0 == m_vec_listLabel.at(2)->text().size())
            m_scrollBar.hide();
    }
}
void Show::setAlignment(int type)
{
    switch(type)
    {
    case 1:
    {
        for(int i = 0; i < 4; i++)
            m_vec_listLabel.at(i)->setAlignment(Qt::AlignLeft);
        break;
    }
    case 2:
    {
        for(int i = 0; i < 4; i++)
            m_vec_listLabel.at(i)->setAlignment(Qt::AlignCenter);
        break;
    }
    case 3:
    {
        for(int i = 0; i < 4; i++)
            m_vec_listLabel.at(i)->setAlignment(Qt::AlignRight);
        break;
    }
    }
}

void Show::setMediaTrack(bool isShow, QString text)
{
    if(isShow)
        m_lab_mediaTrack.setText(text);
    else
        m_lab_mediaTrack.clear();
}
void Show::setMediaClock(bool isShow, QString text)
{
    if(isShow)
        m_lab_mediaClock.setText(text);
    else
        m_lab_mediaClock.clear();
}

void Show::setSoftButtons(std::vector<SSoftButton> vec_softButtons)
{
    m_vec_softButtons = vec_softButtons;

    m_btn_one->setText("-");
    m_btn_two->setText("-");
    m_btn_thr->setText("-");

    m_btn_one->setId(0);
    m_btn_two->setId(0);
    m_btn_thr->setId(0);

    for(int i = 0; i < m_vec_softButtons.size(); i++)
    {
        if(0 == i)
        {
            m_btn_one->setText(m_vec_softButtons.at(i).str_text.data());
            m_btn_one->setId(m_vec_softButtons.at(i).i_softButtonID);
            if(m_vec_softButtons.at(i).b_isHighlighted)
            {
                m_btn_one->setIconNormal(":/images/highlightsoftbutton_mfd.png");
                m_btn_one->setIconPressed(":/images/highlightsoftbutton_mfd.png");
            }
            else
            {
                m_btn_one->setIconNormal(":/images/softbutton_mfd.png");
                m_btn_one->setIconPressed(":/images/softbutton_mfd.png");
            }
        }
        else if(1 == i)
        {
            m_btn_two->setText(m_vec_softButtons.at(i).str_text.data());
            m_btn_two->setId(m_vec_softButtons.at(i).i_softButtonID);
            if(m_vec_softButtons.at(i).b_isHighlighted)
            {
                m_btn_two->setIconNormal(":/images/highlightsoftbutton_mfd_left.png");
                m_btn_two->setIconPressed(":/images/highlightsoftbutton_mfd_left.png");
            }
            else
            {
                m_btn_two->setIconNormal(":/images/softbutton_mfd_left.png");
                m_btn_two->setIconPressed(":/images/softbutton_mfd_left.png");
            }
        }
        else if(2 == i)
        {
            m_btn_thr->setText(m_vec_softButtons.at(i).str_text.data());
            m_btn_thr->setId(m_vec_softButtons.at(i).i_softButtonID);
            if(m_vec_softButtons.at(i).b_isHighlighted)
            {
                m_btn_thr->setIconNormal(":/images/highlightsoftbutton_mfd_right.png");
                m_btn_thr->setIconPressed(":/images/highlightsoftbutton_mfd_right.png");
            }
            else
            {
                m_btn_thr->setIconNormal(":/images/softbutton_mfd.png");
                m_btn_thr->setIconPressed(":/images/softbutton_mfd.png");
            }
        }
    }

    m_i_currentNo = 1;

    if(3 >= m_vec_softButtons.size())
    {
        m_i_totalNum = 1;
        m_btn_fou->changeToExtra(false);
        m_btn_fou->setText("More...");
    }
    else
    {
        m_i_totalNum = m_vec_softButtons.size()/3 + 1;
        m_btn_fou->setText("");

        m_btn_fou->changeToExtra(true);
        m_btn_fou->setIconExtra(":/images/rightarrow.png");
    }

}

//向上滚动
void Show::upArrowSlots()
{
    m_scrollBar.flushScroll(1,2);
    m_listWidget.setCurrentRow(0);
}

//向下滚动
void Show::downArrowSlots()
{
    m_scrollBar.flushScroll(2,2);
    m_listWidget.setCurrentRow(3);
}

void Show::moveBackSlots()
{
    this->close();
    emit returnAppLink();
}

void Show::onListSelect(const QModelIndex &index)
{
    emit this->onListClicked(index.row());
}

void Show::btnFourClickedSlots()
{
    if(m_i_totalNum == 1 && m_i_currentNo == 1)
    {
        //clicked More;
        emit moreClicked();
    }
    else
    {
        m_i_currentNo++;
        if(m_i_currentNo > m_i_totalNum)
            m_i_currentNo = 1;

        m_btn_one->setText("-");
        m_btn_one->setId(0);
        m_btn_two->setText("-");
        m_btn_two->setId(0);
        m_btn_thr->setText("More...");
        m_btn_thr->setId(0);

        for(int i = (m_i_currentNo-1)*3; i < m_vec_softButtons.size(); i++)
        {
            if((m_i_currentNo-1)*3 == i)
            {
                m_btn_one->setText(m_vec_softButtons.at(i).str_text.data());
                m_btn_one->setId(m_vec_softButtons.at(i).i_softButtonID);
                if(m_vec_softButtons.at(i).b_isHighlighted)
                {
                    m_btn_one->setIconNormal(":/images/highlightsoftbutton_mfd.png");
                    m_btn_one->setIconPressed(":/images/highlightsoftbutton_mfd.png");
                }
                else
                {
                    m_btn_one->setIconNormal(":/images/softbutton_mfd.png");
                    m_btn_one->setIconPressed(":/images/softbutton_mfd.png");
                }
            }
            else if((m_i_currentNo-1)*3 + 1 == i)
            {
                m_btn_two->setText(m_vec_softButtons.at(i).str_text.data());
                m_btn_two->setId(m_vec_softButtons.at(i).i_softButtonID);
                if(m_vec_softButtons.at(i).b_isHighlighted)
                {
                    m_btn_two->setIconNormal(":/images/highlightsoftbutton_mfd.png");
                    m_btn_two->setIconPressed(":/images/highlightsoftbutton_mfd.png");
                }
                else
                {
                    m_btn_two->setIconNormal(":/images/softbutton_mfd.png");
                    m_btn_two->setIconPressed(":/images/softbutton_mfd.png");
                }
            }
            else if((m_i_currentNo-1)*3 + 2 == i)
            {
                m_btn_thr->setText(m_vec_softButtons.at(i).str_text.data());
                m_btn_thr->setId(m_vec_softButtons.at(i).i_softButtonID);
                if(m_vec_softButtons.at(i).b_isHighlighted)
                {
                    m_btn_thr->setIconNormal(":/images/highlightsoftbutton_mfd.png");
                    m_btn_thr->setIconPressed(":/images/highlightsoftbutton_mfd.png");
                }
                else
                {
                    m_btn_thr->setIconNormal(":/images/softbutton_mfd.png");
                    m_btn_thr->setIconPressed(":/images/softbutton_mfd.png");
                }
            }
        }
    }


}

void Show::btnThrClickedSlots(int btID)
{
    if(m_i_totalNum == m_i_currentNo && m_i_totalNum != 1)
    {
        //clicked More;
        emit moreClicked();
    }
    else
    {
        emit softButtonClicked(btID, 0);
    }
}

void Show::btnTwoClickedSlots(int btID)
{
    if(btID != 0)
    {
        emit softButtonClicked(btID, 0);
    }
}

void Show::btnOneClickedSlots(int btID)
{
    if(btID != 0)
    {
        emit softButtonClicked(btID, 0);
    }
}

void Show::btnThrClickedLongSlots(int btID)
{
    if(btID != 0)
    {
        emit softButtonClicked(btID, 1);
    }
}

void Show::btnTwoClickedLongSlots(int btID)
{
    if(btID != 0)
    {
        emit softButtonClicked(btID, 1);
    }
}

void Show::btnOneClickedLongSlots(int btID)
{
    if(btID != 0)
    {
        emit softButtonClicked(btID, 1);
    }
}

void Show::execShow(AppDataInterface* pAppInterface)
{
    Json::Value pObj;
    std::vector <SSoftButton > vec_softButtons;
    vec_softButtons.clear();
    this->setMainField1(true,"");
    this->setMainField2(true,"");
    this->setMainField3(true,"");
    this->setMainField4(true,"");
    this->setMediaTrack(true,"");
    this->setMediaClock(true,"");
    if (pAppInterface)
    {
        pObj = pAppInterface->getShowData();
        for(int i = 0; i < pObj["params"]["showStrings"].size(); i++)
        {
            if("mainField1" == pObj["params"]["showStrings"][i]["fieldName"].asString())
            {
                this->setMainField1(true,pObj["params"]["showStrings"][i]["fieldText"].asString().data());
            }
            else if("mainField2" == pObj["params"]["showStrings"][i]["fieldName"].asString())
            {
                this->setMainField2(true,pObj["params"]["showStrings"][i]["fieldText"].asString().data());
            }
            else if("mainField3" == pObj["params"]["showStrings"][i]["fieldName"].asString())
            {
                this->setMainField3(true,pObj["params"]["showStrings"][i]["fieldText"].asString().data());
            }
            else if("mainField4" == pObj["params"]["showStrings"][i]["fieldName"].asString())
            {
                this->setMainField4(true,pObj["params"]["showStrings"][i]["fieldText"].asString().data());
            }
            else if("mediaTrack" == pObj["params"]["showStrings"][i]["fieldName"].asString())
            {
                this->setMediaTrack(true,pObj["params"]["showStrings"][i]["fieldText"].asString().data());
            }
            else if("mediaClock" == pObj["params"]["showStrings"][i]["fieldName"].asString())
            {
                this->setMediaClock(true,pObj["params"]["showStrings"][i]["fieldText"].asString().data());
            }
        }
        if(pObj["params"].isMember("softButtons"))
        {

            for(int i = 0; i < pObj["params"]["softButtons"].size(); i++)
            {
                SSoftButton tmpSoftButton;
                tmpSoftButton.b_isHighlighted = pObj["params"]["softButtons"][i]["isHighlighted"].asBool();
                tmpSoftButton.i_softButtonID = pObj["params"]["softButtons"][i]["softButtonID"].asInt();
                tmpSoftButton.str_text = pObj["params"]["softButtons"][i]["text"].asString();
                vec_softButtons.push_back(tmpSoftButton);
            }
            this->setSoftButtons(vec_softButtons);
        }
    }
    this->show();
}

//{
//   "id" : 78,
//   "jsonrpc" : "2.0",
//   "method" : "UI.SetMediaClockTimer",
//   "params" : {
//      "appID" : 424238335,
//      "endTime" : {
//         "hours" : 11,
//         "minutes" : 22,
//         "seconds" : 33
//      },
//      "startTime" : {
//         "hours" : 0,
//         "minutes" : 12,
//         "seconds" : 34
//      },
//      "updateMode" : "COUNTUP"
//   }
//}
void Show::setMediaColckTimer(Json::Value jsonObj)
{
    m_i_startH = jsonObj["params"]["startTime"]["hours"].asInt();
    m_i_startM = jsonObj["params"]["startTime"]["minutes"].asInt();
    m_i_startS = jsonObj["params"]["startTime"]["seconds"].asInt();
    m_i_endH = jsonObj["params"]["endTime"]["hours"].asInt();
    m_i_endM = jsonObj["params"]["endTime"]["minutes"].asInt();
    m_i_endS = jsonObj["params"]["endTime"]["seconds"].asInt();

    if(jsonObj["params"]["updateMode"].asString() == "COUNTUP")
    {
        nowMeidaClockTime.setHMS(m_i_startH, m_i_startM, m_i_startS);
        m_b_countup = true;
        m_timer_mediaClock->start();
    }
    else if(jsonObj["params"]["updateMode"].asString() == "COUNTDOWN")
    {
        m_b_countup = false;
        nowMeidaClockTime.setHMS(m_i_startH, m_i_startM, m_i_startS);
        m_timer_mediaClock->start();
    }
    else if(jsonObj["params"]["updateMode"].asString() == "PAUSE")
    {
        m_timer_mediaClock->stop();

    }
    else if(jsonObj["params"]["updateMode"].asString() == "RESUME")
    {
        m_timer_mediaClock->start();

    }
    else if(jsonObj["params"]["updateMode"].asString() == "CLEAR")
    {
        m_timer_mediaClock->stop();
        this->setMediaClock(true,"");
    }
}

void Show::mediaClockSlots()
{
    if(m_b_countup)
    {
        nowMeidaClockTime = nowMeidaClockTime.addSecs(1);
    }
    else
    {
        nowMeidaClockTime = nowMeidaClockTime.addSecs(-1);
    }

    if(nowMeidaClockTime.hour() == m_i_endH
            && nowMeidaClockTime.minute() == m_i_endM
            && nowMeidaClockTime.second() == m_i_endS)
        m_timer_mediaClock->stop();

    this->setMediaClock(true,nowMeidaClockTime.toString());
}
