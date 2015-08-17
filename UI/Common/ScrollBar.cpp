#include "ScrollBar.h"

ScrollBar::ScrollBar(QWidget *parent) : QWidget(parent)
{

}

ScrollBar::~ScrollBar()
{

}

void ScrollBar::init(int rowNo, int h)
{
    m_i_rowNo = rowNo;

    m_i_scrollW = 18;
    m_i_scrollH = h-70;
    m_i_startX = 15;
    m_i_startY = 35;

    this->setFixedSize(50,h);

    QVBoxLayout *mLayout = new QVBoxLayout(this);
    mLayout->addWidget(&m_btn_upArrow,10,Qt::AlignTop);
    mLayout->addWidget(&m_lab_scroll,80,Qt::AlignCenter);
    mLayout->addWidget(&m_btn_downArrow,10,Qt::AlignBottom);

    m_btn_upArrow.setFixedSize(30,25);
    m_btn_downArrow.setFixedSize(30,25);
//    m_lab_scroll.setFixedSize(30,h-50);
//    m_btn_upArrow.setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
//    m_btn_downArrow.setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    QPixmap pixmapUp(":/images/uparrow.png");
    QPixmap fitpixmapUp = pixmapUp.scaled(28, 25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_btn_upArrow.setIcon(QIcon(fitpixmapUp));
    m_btn_upArrow.setIconSize(QSize(28, 25));
    m_btn_upArrow.setFlat(true);
    m_btn_upArrow.setStyleSheet("border: 0px");

    QPixmap pixmapDown(":/images/downarrow.png");
    QPixmap fitpixmapDown = pixmapDown.scaled(28, 25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_btn_downArrow.setIcon(QIcon(fitpixmapDown));
    m_btn_downArrow.setIconSize(QSize(28, 25));
    m_btn_downArrow.setFlat(true);
    m_btn_downArrow.setStyleSheet("border: 0px");

    connect(&m_btn_upArrow,SIGNAL(clicked()),this,SIGNAL(upClicked()));
    connect(&m_btn_downArrow,SIGNAL(clicked()),this,SIGNAL(downClicked()));

}

void ScrollBar::flushScroll(int currentNo, int totalNum)
{
    m_i_totalNum = totalNum;
    m_i_currentNo = currentNo;

    this->repaint();
}

void ScrollBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(108,113,125)));
    painter.setPen(Qt::NoPen);

    int rectH = m_i_scrollH / m_i_totalNum;

    for(int i = 0; i < m_i_totalNum; i++)
    {
        if(i+1 >= m_i_currentNo && i+1 <= m_i_currentNo+m_i_rowNo-1)
        {
            painter.setBrush(QBrush(QColor(108,113,125)));
        }
        else
        {
            painter.setBrush(QBrush(QColor(63, 70, 87)));
        }
        painter.setPen(Qt::NoPen);
        painter.drawRect(m_i_startX, m_i_startY + i*rectH, m_i_scrollW, rectH-1);
    }
}

void ScrollBar::mousePressEvent(QMouseEvent *e)
{
    m_i_clickX = e->x();
    m_i_clickY = e->y();
    //点击不触发事件，释放按钮才触发事件；

}

void ScrollBar::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()== Qt::LeftButton)
    {
//        if((m_i_clickX == e->x()) && (m_i_clickY == e->y()))
        {
            //emit clicked();
            int rectH = m_i_scrollH / m_i_totalNum;
            //qDebug()<<m_i_clickY/rectH <<m_i_currentNo;
            if(m_i_clickY/rectH < m_i_currentNo )
                emit upClicked();
            else if(m_i_clickY/rectH > m_i_currentNo + 3)
                emit downClicked();

            m_i_clickX = 0;
            m_i_clickY = 0;
        }
    }
}


