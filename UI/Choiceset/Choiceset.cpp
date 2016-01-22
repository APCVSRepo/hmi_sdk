#include "Choiceset.h"

Choiceset::Choiceset(AppListInterface * pList, QWidget *parent) : AppBase(pList, parent)
{
    m_listWidget = new AppListWidget(ui_app_width*0.05,5,ui_app_width*0.9,ui_app_height-10,this);
    m_timerHide = new QTimer;

    initLayout();

    connect(m_timerHide,SIGNAL(timeout()),this,SLOT(timeHideOutSlots()));
    connect(m_listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(listWidgetDoubleClickedSlots(QModelIndex)));
    connect(this,SIGNAL(menuClicked(int,int)),this,SLOT(menuClickedSlots(int,int)));
}

Choiceset::~Choiceset()
{
    delete m_timerHide;
}

void Choiceset::initLayout()
{
    QHBoxLayout *midLayout = new QHBoxLayout;
    midLayout->addStretch(2);
    midLayout->addWidget(m_listWidget,90,Qt::AlignCenter);
//    midLayout->addWidget(&m_scrollBar,5);
    midLayout->addStretch(3);
    QVBoxLayout *mLayout = new QVBoxLayout(this);
    mLayout->addLayout(midLayout, 4);

    mLayout->setMargin(0); //边框无缝
    mLayout->setSpacing(0);

    m_listWidget->setFixedSize(ui_app_width*0.9,ui_app_height-10);
}



//双击某行，可进行某一行的子菜单;
void Choiceset::listWidgetDoubleClickedSlots(QModelIndex index)
{
    m_timerHide->stop();
    emit menuClicked(PERFORMINTERACTION_CHOICE, index.row());
}

//增加Command，增加显示到list的最上一行;
void Choiceset::addNewMenu(int choiceID, std::string menuName)
{
    SChoiceMenu tmpChoiceMenu;
    tmpChoiceMenu.i_choiceID = choiceID;
    tmpChoiceMenu.str_menuName = menuName;
    m_vec_choiceMenu.append(tmpChoiceMenu);
    m_vec_isMenu.append(false);
}

//刷新list列表
void Choiceset::flushListWidget()
{
    m_listWidget->DelListItemWidget();
    m_listWidget->SetScrollParams(4,m_vec_choiceMenu.size());
    for(int i = 0; i < m_vec_choiceMenu.size(); i++)
        m_listWidget->AddListItemWidget(QString("%1 %2 %3").arg(i).arg(m_vec_choiceMenu.at(i).str_menuName.data()).arg(m_vec_choiceMenu.at(i).i_choiceID),"",m_vec_isMenu.at(i));


	m_listWidget->FlushAll();
}


void Choiceset::menuClickedSlots(int code,int row)
{
    //_D("code=%d:%d:%d\n",code,performInteractionID,choiceID);
    AppControl->OnPerformInteraction(code, row);
}

void Choiceset::setChoicesetName(QString title)
{
    this->setTitle(title);
}


void Choiceset::setTimeOut(int duration)
{
    m_timerHide->start(duration);
}

void Choiceset::timeHideOutSlots()
{
    m_timerHide->stop();
    emit menuClicked(PERFORMINTERACTION_TIMEOUT, 0);
}

void Choiceset::ClearAllItem()
{
    m_vec_choiceMenu.clear();
    m_vec_isMenu.clear();
}

void Choiceset::showEvent(QShowEvent * e)
{
    setChoicesetName("Choice Name");
    ClearAllItem();
    if (AppControl)
    {
        Json::Value jsonData = AppControl->getInteractionJson();
        Json::Value jsonChoice= jsonData["Choiceset"];
        //setChoicesetName(jsonChoice["vrHelpTitle"].asString().c_str());
        if(jsonChoice.isMember("timeout"))
            setTimeOut(jsonChoice["timeout"].asInt());
        if(jsonChoice.isMember("choiceSet")){
            for(unsigned int i = 0; i < jsonChoice["choiceSet"].size(); i++)
            {
                addNewMenu(jsonChoice["choiceSet"][i]["choiceID"].asInt(),jsonChoice["choiceSet"][i]["menuName"].asString());
            }
        }

        flushListWidget();
    }
}
