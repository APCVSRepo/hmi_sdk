#include "Show.h"
#include "UI/Command/Command.h"

Show::Show(AppListInterface * pList, QWidget *parent) : AppBase(pList, parent)
{
    m_btn_one = new CButton;
    m_btn_two = new CButton;
    m_btn_thr = new CButton;
    m_btn_fou = new CButton;
    m_listWidget = new AppListWidget(ui_app_width*0.1,0,ui_app_width*2.0/3.0,ui_app_height*3.0/4.0,NULL);

    initLayout();
    m_i_totalNum = 1;
    m_i_currentNo = 1;
    m_timerId=0;

    connect(this,SIGNAL(startMediaClock(bool)),SLOT(mediaClockSlots(bool)));
    connect(m_listWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListSelect(QModelIndex)));
   // m_timer_mediaClock = new QTimer;
    //m_timer_mediaClock->setInterval(1000);
   // connect(m_timer_mediaClock,SIGNAL(timeout()),this,SLOT(mediaClockSlots()));
}

Show::~Show()
{
    delete m_btn_one;
    delete m_btn_two;
    delete m_btn_thr;
    delete m_btn_fou;
    delete m_listWidget;
}

void Show::initLayout()
{
    QHBoxLayout *hhLayout = new QHBoxLayout;
    //hhLayout->addStretch(1);
    hhLayout->addWidget(&m_lab_mediaTrack,4,Qt::AlignLeft|Qt::AlignBottom);
    hhLayout->addWidget(&m_lab_mediaClock,4,Qt::AlignRight|Qt::AlignBottom);
    hhLayout->addStretch(2);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(m_listWidget,80,Qt::AlignCenter);
    vLayout->addLayout(hhLayout,20);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(&m_lab_icon,25,Qt::AlignLeft);
    hLayout->addLayout(vLayout,75);
   // hLayout->addWidget(&m_scrollBar,10);
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_btn_one,1,Qt::AlignCenter);
    btnLayout->addWidget(m_btn_two,1,Qt::AlignCenter);
    btnLayout->addWidget(m_btn_thr,1,Qt::AlignCenter);
    btnLayout->addWidget(m_btn_fou,1,Qt::AlignCenter);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(hLayout,3);
    mainLayout->addLayout(btnLayout,1);

    QPixmap pixmap(":/images/sync.png");
    QPixmap fitpixmap=pixmap.scaled(120,120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_lab_icon.setPixmap(fitpixmap);

    m_btn_one->initParameter(ui_btn_width,ui_btn_height,":/images/softbutton_mfd.png",":/images/softbutton_mfd.png");
    m_btn_two->initParameter(ui_btn_width,ui_btn_height,":/images/softbutton_mfd_left.png",":/images/softbutton_mfd_left.png");
    m_btn_thr->initParameter(ui_btn_width,ui_btn_height,":/images/softbutton_mfd_right.png",":/images/softbutton_mfd_right.png");
    m_btn_fou->initParameter(ui_btn_width,ui_btn_height,":/images/softbutton_mfd.png",":/images/softbutton_mfd.png");
    m_btn_one->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btn_two->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btn_thr->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btn_fou->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");


    m_listWidget->setFixedSize(ui_app_width*2.0/3.0,ui_app_height*3.0/4.0);
    m_listWidget->SetScrollParams(1,1);

//    m_listWidget->AddListItemWidget("",false);
//    m_listWidget->AddListItemWidget("",false);
//    m_listWidget->AddListItemWidget("",false);
//    m_listWidget->AddListItemWidget("",false);
    m_lab_mediaTrack.setStyleSheet("font: 75 36pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");
    m_lab_mediaClock.setStyleSheet("font: 75 36pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");


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

    m_vec_softButtons.clear();
}


void Show::setMediaTrack(bool isShow, QString text)
{
    if(isShow){
        m_lab_mediaTrack.show();
        SetEdlidedText(&m_lab_mediaTrack,text,ui_app_width / 2.0);
    }
    else{
        m_lab_mediaTrack.hide();
    }
}
void Show::setMediaClock(bool isShow, QString text)
{
    LOGI(text.toStdString().data());
    if(isShow){
        m_lab_mediaClock.show();
        SetEdlidedText(&m_lab_mediaClock,text,ui_app_width / 2.0);
    }
    else{
        m_lab_mediaClock.hide();
    }
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

    for(unsigned int i = 0; i < m_vec_softButtons.size(); i++)
    {
        if(0 == i)
        {
            m_btn_one->setText(m_vec_softButtons.at(i).str_text.c_str());
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
            m_btn_two->setText(m_vec_softButtons.at(i).str_text.c_str());
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
            m_btn_thr->setText(m_vec_softButtons.at(i).str_text.c_str());
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


void Show::onListSelect(const QModelIndex &index)
{
    emit this->onListClicked(index.row());
}

void Show::btnFourClickedSlots()
{
    if(m_i_totalNum == 1 && m_i_currentNo == 1)
    {
        //clicked More;
        AppControl->OnShowCommand();
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

        for(unsigned int i = (m_i_currentNo-1)*3; i < m_vec_softButtons.size(); i++)
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
        AppControl->OnShowCommand();
    }
    else
    {
        AppControl->OnSoftButtonClick(btID, 0);
    }
}

void Show::btnTwoClickedSlots(int btID)
{
    if(btID != 0)
    {
        AppControl->OnSoftButtonClick(btID, 0);
    }
}

void Show::btnOneClickedSlots(int btID)
{
    if(btID != 0)
    {
        AppControl->OnSoftButtonClick(btID, 0);
    }
}

void Show::btnThrClickedLongSlots(int btID)
{
    if(btID != 0)
    {
        AppControl->OnSoftButtonClick(btID, 1);
    }
}

void Show::btnTwoClickedLongSlots(int btID)
{
    if(btID != 0)
    {
        AppControl->OnSoftButtonClick(btID, 1);
    }
}

void Show::btnOneClickedLongSlots(int btID)
{
    if(btID != 0)
    {
        AppControl->OnSoftButtonClick(btID, 1);
    }
}

void Show::showEvent(QShowEvent * e)
{
    Json::Value pObj;
    std::vector <SSoftButton > vec_softButtons;
    vec_softButtons.clear();
    if (AppControl)
    {
        pObj = AppControl->getShowData();
		if(pObj.isNull())
			return;
		Json::Value jsonParams = pObj["params"];
		int fieldNum=0;
		QString fieldText[4];
		bool    mediaHas=false;
		for (unsigned int i = 0; i < jsonParams["showStrings"].size(); i++)
		{
			Json::Value  fieldName = jsonParams["showStrings"][i];
			if ("mainField1" == fieldName["fieldName"].asString())
			{
				fieldText[fieldNum++] = fieldName["fieldText"].asString().data();
			}
			else if ("mainField2" == fieldName["fieldName"].asString())
			{
				fieldText[fieldNum++] = fieldName["fieldText"].asString().data();
			}
			else if ("mainField3" == fieldName["fieldName"].asString())
			{
				fieldText[fieldNum++] = fieldName["fieldText"].asString().data();
			}
			else if ("mainField4" == fieldName["fieldName"].asString())
			{
				fieldText[fieldNum++] = fieldName["fieldText"].asString().data();
			}
			else if ("mediaTrack" == fieldName["fieldName"].asString())
			{
				mediaHas = true;
				this->setMediaTrack(true, fieldName["fieldText"].asString().data());
			}
			else if ("mediaClock" == fieldName["fieldName"].asString())
			{
				mediaHas = true;
				this->setMediaClock(true, fieldName["fieldText"].asString().data());
			}
		}
		m_listWidget->DelListItemWidget();
		m_listWidget->setFixedSize(ui_app_width*2.0 / 3.0, ui_app_height*(mediaHas ? 2.0 : 3.0) / 4.0);
		if (!mediaHas){
			this->setMediaTrack(false, "");
			this->setMediaClock(false, "");
		}
		m_listWidget->SetScrollParams(mediaHas ? 2 : fieldNum, fieldNum);
		for (int i = 0; i<fieldNum; i++){
			m_listWidget->AddListItemWidget(fieldText[i], false);
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
			this->setSoftButtons(vec_softButtons);
		}
    }
    
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

void Show::UpdateMediaColckTimer()
{
    Json::Value jsonObj = AppControl->getMediaClockJson();

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
        emit startMediaClock(true);
    }
    else if(jsonObj["params"]["updateMode"].asString() == "COUNTDOWN")
    {
        m_b_countup = false;
        nowMeidaClockTime.setHMS(m_i_startH, m_i_startM, m_i_startS);
        emit startMediaClock(true);
    }
    else if(jsonObj["params"]["updateMode"].asString() == "PAUSE")
    {
        emit startMediaClock(false);
    }
    else if(jsonObj["params"]["updateMode"].asString() == "RESUME")
    {
        emit startMediaClock(true);
    }
    else if(jsonObj["params"]["updateMode"].asString() == "CLEAR")
    {
        emit startMediaClock(false);
        this->setMediaClock(true,"");
    }
}

void Show::timerEvent(QTimerEvent *e)
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
        this->killTimer(m_timerId);

    this->setMediaClock(true,nowMeidaClockTime.toString("HH:mm:ss"));
}

void Show::mediaClockSlots(bool isStart)
{
  if(isStart){
    if(m_timerId!=0)
      this->killTimer(m_timerId);
    m_timerId=this->startTimer(1000);
  }
  else{
      if(m_timerId!=0)
        this->killTimer(m_timerId);
  }
}
