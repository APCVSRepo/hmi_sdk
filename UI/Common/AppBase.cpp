#include "AppBase.h"
extern Config g_config;

AppBase::AppBase(QWidget *parent) : QWidget(parent)
{

    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setGeometry(0,0,g_config.m_i_mainWindowW,g_config.m_i_mainWindowH);
//    this->setFixedSize(g_config.m_i_mainWindowW,g_config.m_i_mainWindowH);
    this->setAutoFillBackground(true);    //Widget增加背景图片时，这句一定要。
    QPixmap pixmap(":/images/mfd5main.png");
    QPixmap fitpixmap=pixmap.scaled(g_config.m_i_mainWindowW,g_config.m_i_mainWindowH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(fitpixmap));
    this->setPalette(palette);

    //显示日期时间
    m_timer=new QTimer(this);
    m_timer->start(1000);
    GetDateTime();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(GetDateTime()));

    m_lab_title.setText("Mobile Apps");
    m_lab_title.setStyleSheet("font: 75 40pt \"Liberation Serif\";color:rgb(13,193,226);border: 0px");
    m_lab_time.setStyleSheet("font: 75 40pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");

    m_i_leftRight = 2;
    m_i_count = 0;


    btnLayout = new QVBoxLayout;
    btnLayout->addStretch(2);
    btnLayout->addWidget(&m_btn_FM,3, Qt::AlignCenter);
    btnLayout->addWidget(&m_btn_Tel,3, Qt::AlignCenter);
    btnLayout->addWidget(&m_btn_Msg,3, Qt::AlignCenter);
    btnLayout->addWidget(&m_btn_CD,3, Qt::AlignCenter);
    btnLayout->addWidget(&m_btn_List,3, Qt::AlignCenter);
    btnLayout->addStretch(1);

    menuLayout = new QHBoxLayout;
    menuLayout->addLayout(btnLayout,80);
    menuLayout->addStretch(20);

    m_btn_FM.setFixedSize(50,50);
    m_btn_Tel.setFixedSize(50,50);
    m_btn_Msg.setFixedSize(50,50);
    m_btn_CD.setFixedSize(50,50);
    m_btn_List.setFixedSize(50,50);

    m_btn_FM.setFlat(true);
    m_btn_Tel.setFlat(true);
    m_btn_Msg.setFlat(true);
    m_btn_CD.setFlat(true);
    m_btn_List.setFlat(true);

    m_btn_FM.setStyleSheet("border:0px");
    m_btn_Tel.setStyleSheet("border:0px");
    m_btn_Msg.setStyleSheet("border:0px");
    m_btn_CD.setStyleSheet("border:0px");
    m_btn_List.setStyleSheet("border:0px");

    connect(&m_btn_FM,SIGNAL(clicked()),this,SLOT(btnFMClickedSlots()));
    connect(&m_btn_Tel,SIGNAL(clicked()),this,SLOT(btnTelClickedSlots()));
    connect(&m_btn_Msg,SIGNAL(clicked()),this,SLOT(btnMsgClickedSlots()));
    connect(&m_btn_CD,SIGNAL(clicked()),this,SLOT(btnCDClickedSlots()));
    connect(&m_btn_List,SIGNAL(clicked()),this,SLOT(btnListClickedSlots()));

}

AppBase::~AppBase()
{

}

//获取当前时间的时和分;
void AppBase::GetDateTime()
{
    QTime timeNow=QTime::currentTime();
    m_lab_time.setText(tr("%1").arg(timeNow.toString()).left(5)+"  ");
}

void AppBase::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<"("+QString::number(event->x())+","+QString::number(event->y())+")";

    static int m_i_tmpX=0;
    if(event->buttons()&Qt::LeftButton)
    {
//        qDebug()<<"鼠标左键按下:移动"<<m_i_tmpX<< event->x();

        if(m_i_tmpX != 0 && m_i_tmpX < event->x())
        {
            //moveAllBtnRight();
            //qDebug()<<"鼠标左键按下:右移动";
            m_i_count++;
            if(m_i_count > 10)
            {
                m_i_count = 0;
                m_i_leftRight = 0;
            }
        }
        else if(m_i_tmpX != 0 && m_i_tmpX > event->x())
        {
            //moveAllBtnLeft();
            //qDebug()<<"鼠标左键按下:左移动";
            m_i_count++;
            if(m_i_count > 10)
            {
                m_i_count = 0;
                m_i_leftRight = 1;
            }
        }

        m_i_tmpX = event->x();
    }
}
void AppBase::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_i_leftRight == 0)
    {
//        emit moveBack();
    }
    else if(m_i_leftRight == 1)
    {

    }
    m_i_leftRight = 2;
    m_i_count = 0;
}

void AppBase::execShow(AppDataInterface* pAppInterface)
{

}

void AppBase::btnFMClickedSlots()
{
    emit menuBtnClicked("FMButton");
}
void AppBase::btnTelClickedSlots()
{
    emit menuBtnClicked("TelButton");
}
void AppBase::btnMsgClickedSlots()
{
    emit moveBack();
//    emit menuBtnClicked("MsgButton");
}
void AppBase::btnCDClickedSlots()
{
    emit menuBtnClicked("CDButton");
}
void AppBase::btnListClickedSlots()
{
    emit menuBtnClicked("ListButton");
}

