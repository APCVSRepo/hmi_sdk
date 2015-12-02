#include "MainMenue.h"
#include "../Config/Config.h"
#include "AppBase.h"
#include "PopBase.h"
#include <QTime>
MainMenue::MainMenue(QWidget *parent) : BaseWidght(0,0,ui_res_width,ui_res_height,parent)
{
    this->setBackGroundImge((char *)":/images/mainbkg.png");

    labelTitle=new QLabel(this);
    labelTitle->setGeometry(ui_res_width*1/3.0,5,ui_res_width*1/3.0,ui_res_height*ui_menu_udmargin-10);
    labelTitle->setText("Mobile Apps");

    labelTitle->setStyleSheet(QString("font: 60 35pt \"Liberation Serif\";color:rgb(13,193,226);border: 0px"));
   // labelTitle->setStyleSheet("font: 75 40pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");

    labelTime=new QLabel(this);
    labelTime->setGeometry(ui_res_width*(5.0/6.0)-ui_content_lr,ui_res_height*(1-ui_menu_udmargin)+3,ui_res_width*1/6.0,ui_res_height*ui_menu_udmargin-5);
    //labelTime->setText("Mobile Apps");
   // labelTime->setStyleSheet("font: 75 40pt \"Liberation Serif\";color:rgb(13,193,226);border: 0px");
    labelTime->setStyleSheet("font: 55 30pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");
    //显示日期时间
    m_timer=new QTimer(this);
    m_timer->start(1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(GetDateTime()));



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

    connect(this,SIGNAL(menuBtnClicked(QString)),this,SLOT(menuBtnClickedSlots(QString)));

}

MainMenue::~MainMenue()
{

}

void MainMenue::SetTitle(QString title)
{
    this->labelTitle->setText(title);
}

//param: histroy,是否加入历史记录
void MainMenue::SetCurWidget(int id,bool history)
{
    LOGD("cur widght:id=%d,index=%d\n",id,stackKeys.key(id));
    stackWidget->setCurrentIndex(stackKeys.key(id));
    if(id>=MIN_APP_BASE && id<MAX_APP_BASE){
        LOGD("cur widght:AppBase\n");
        AppBase *app=static_cast<AppBase*>(stackWidget->currentWidget());
        app->execShow();
    }
    else
    {
        LOGD("cur widght:PopBase\n");
        CPopBase *pop=static_cast<CPopBase*>(stackWidget->currentWidget());
        pop->execShow();
    }
    if(history){
       if(stackHistory.isEmpty())
          stackHistory.append(id);
       else{
           if(stackHistory.back()!=id)
               stackHistory.append(id);
       }
    }
}

void MainMenue::InserWidget(int id,QWidget *widget)
{
    stackKeys.insert(stackWidget->count(),id);
    stackWidget->addWidget(widget);
}

QWidget* MainMenue::CenterWidght()
{
    return stackWidget;
}

void MainMenue::ReceiveJson(int id, Json::Value json)
{
    AppBase *app=static_cast<AppBase*>(stackWidget->widget(stackKeys.key(id)));
    if(app)
        app->receiveJson(json);
}

void MainMenue::ExitWidget(int id)
{
    int index=stackKeys.key(id);
    if(stackWidget->currentIndex()==index){
        this->onMoveBack();
    }
}

void MainMenue::GetDateTime()
{
    this->labelTime->setText(QTime::currentTime().toString("HH:mm:ss"));
}

void MainMenue::onMenuButtonClick()
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
                stackHistory.clear();
                this->SetCurWidget(ID_APPLINK);
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

void MainMenue::menuBtnClickedSlots(QString btnText)
{

    //_D("%s\n",btnText.toUtf8().data());

    DataManager::DataInterface()->OnMenuBtnClick(btnText.toUtf8().data());

    if("ListButton" == btnText)
    {
        this->SetCurWidget(ID_AUDIOPASSTHRU);
        //ts.speak("请说一个指令");
        //while(ts.isSpeaking())
           // waitMSec(100);
        DataManager::DataInterface()->OnVRStartRecord();
    }
}

void MainMenue::onMoveBack()
{
    LOGD("onMoveBack");
    if(stackHistory.isEmpty()){
        LOGD("onMoveBack:close()\n");
        if(stackKeys.value(stackWidget->currentIndex())==ID_APPLINK)
               this->close();
        else{
            this->SetCurWidget(ID_APPLINK);
        }
    }
    else{
        stackHistory.removeLast();
        if(stackHistory.isEmpty())
        {
            LOGD("stackHistory is empty\n");
            this->SetCurWidget(ID_APPLINK,false);
        }
        else
        {
           int id=stackHistory.back();
           LOGD("stackHistory is id=%d\n",id);
           SetCurWidget(id,false);
        }
    }
}


void MainMenue::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Back){
        this->onMoveBack();
    }
    else{
        QWidget::keyPressEvent(e);
    }
}

