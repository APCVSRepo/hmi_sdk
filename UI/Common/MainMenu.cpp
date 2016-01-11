#include "MainMenu.h"
#include "../Config/Config.h"
#include "AppBase.h"
#include <QTime>
MainMenu::MainMenu(AppListInterface * pList, QWidget *parent) : BaseWidght(0,0,ui_res_width,ui_res_height,parent)
{
    m_pList = pList;
    this->setBackGroundImge((char *)":/images/mainbkg.png");

    labelTitle=new QLabel(this);
    labelTitle->setGeometry(ui_res_width*1/3.0,5,ui_res_width*1/3.0,ui_res_height*ui_menu_udmargin-10);
    labelTitle->setText("Mobile Apps");

    labelTitle->setStyleSheet(QString("font: 60 35pt \"Liberation Serif\";color:rgb(13,193,226);border: 0px"));
   // labelTitle->setStyleSheet("font: 75 40pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");

    labelTime=new QLabel(this);
    labelTime->setGeometry(ui_res_width*(4.0/5.0)-ui_content_lr,ui_res_height*(1-ui_menu_udmargin)+3,ui_res_width*1/5.0,ui_res_height*ui_menu_udmargin-5);
    //labelTime->setText("Mobile Apps");
   // labelTime->setStyleSheet("font: 75 40pt \"Liberation Serif\";color:rgb(13,193,226);border: 0px");
    labelTime->setStyleSheet("font: 55 30pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");
    //显示日期时间
    m_timer=new QTimer(this);
    m_timer->start(1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(ShowDateTime()));

    widgetLeftMenu=new BaseWidght(ui_content_lr,ui_content_ud,ui_leftMenu_Width,ui_content_height,this);
    widgetLeftMenu->setBackGroundImge((char *)":/images/leftmenu.png");

    widgetCenter=new BaseWidght(ui_content_lr+ui_leftMenu_Width,ui_content_ud,ui_content_width,ui_content_height,this);
    widgetCenter->setBackGroundImge((char *)":/images/centerbkg.png");

    btnUpScroll=new QPushButton(widgetLeftMenu);
    btnUpScroll->setGeometry(1,2,ui_leftMenu_SizeW,ui_leftMenu_SizeH);
    btnUpScroll->setStyleSheet((char *)"border-image:url(:/images/muparray.png)");
    btnUpScroll->setFlat(true);
    btnUpScroll->setFocusPolicy(Qt::NoFocus);
    char*  btnImg[ID_MENU_MAX]={(char *)":/images/mf.png",(char *)":/images/tel.png",(char *)":/images/msg.png",(char *)":/images/cd.png",(char *)":/images/cmdlist.png"};
    for(int i=0;i<ID_MENU_MAX;i++)
    {
        btnMenuList[i]=new QPushButton(widgetLeftMenu);
        btnMenuList[i]->setGeometry(1,2+ui_leftMenu_SizeH*(i+1),ui_leftMenu_SizeW,ui_leftMenu_SizeH);
        btnMenuList[i]->setStyleSheet(QString("border-image:url(%1)").arg(btnImg[i]));
        btnMenuList[i]->setFlat(true);
        btnMenuList[i]->setFocusPolicy(Qt::NoFocus);
        connect(btnMenuList[i],SIGNAL(clicked()),SLOT(onMenuButtonClick()));
    }
    stackWidget=new QStackedWidget(widgetCenter);
    stackWidget->setGeometry(ui_content_left,5,ui_app_width,ui_app_height);
    stackWidget->setWindowFlags(Qt::FramelessWindowHint);

    videoWidget = new VideoStream(ui_res_width,ui_res_height);
    connect(this,SIGNAL(menuBtnClicked(QString)),this,SLOT(menuBtnClickedSlots(QString)));
}

MainMenu::~MainMenu()
{
    delete stackWidget;
    delete videoWidget;
}

void MainMenu::SetTitle(QString title)
{
    labelTitle->setText(title);
}


QWidget* MainMenu::CenterWidget()
{
    return stackWidget;
}

void MainMenu::StartVideoStream(const char *url)
{
    videoWidget->setUrl(url);
    videoWidget->startStream();
}

void MainMenu::StopVideoStream()
{
    videoWidget->stopStream();
}

void MainMenu::ShowDateTime()
{
    labelTime->setText(QTime::currentTime().toString("HH:mm:ss"));
}

void MainMenu::onMenuButtonClick()
{
    QPushButton *btn=static_cast<QPushButton*>(sender());
    if(btn==NULL)
        return;
    for(int i=0;i<ID_MENU_MAX;i++){
        if(btnMenuList[i]==btn){
            switch(i)
            {
            case ID_MENU_MF:
                emit menuBtnClicked("FMButton");
                break;
            case ID_MENU_TEL:
                emit menuBtnClicked("TelButton");
                break;
            case ID_MENU_MSG:
                {
                m_pList->ShowAppList();
                }
                break;
            case ID_MENU_CD:
                emit menuBtnClicked("CDButton");
                break;
            case ID_MENU_LIST:
                emit menuBtnClicked("ListButton");
                break;
            }

            break;
        }
    }
}

void MainMenu::menuBtnClickedSlots(QString btnText)
{
    //_D("%s\n",btnText.toUtf8().data());

	if (!m_pList->getActiveApp())
		return;
	m_pList->getActiveApp()->OnMenuBtnClick(btnText.toUtf8().data());

    if("ListButton" == btnText)
    {
        m_pList->getActiveApp()->OnVRStartRecord();
	}
}

void MainMenu::keyPressEvent(QKeyEvent *e)
{
}

