#include "AppItemWidget.h"

AppItemWidget::AppItemWidget(int w,int h,QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(w,h);

  //  this->setStyleSheet("background-color: transparent;");

    m_lab_caseTitle = new QLabel;
    m_lab_icon = new QLabel;
    m_lab_border = new QLabel;

    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addStretch(10);
    mLayout->addWidget(m_lab_caseTitle,80,Qt::AlignLeft);
    mLayout->addWidget(m_lab_icon,10,Qt::AlignCenter);

    m_lab_caseTitle->setStyleSheet("font: 75 36pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");

    QPixmap pixmap(":/images/rightarrow.png");
    QPixmap fitpixmap=pixmap.scaled(30,30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_lab_icon->setPixmap(fitpixmap);

    m_lab_border->setParent(this);
    m_lab_border->setGeometry(2,2,w-4,h-4);
    m_lab_border->setStyleSheet("border: 2px solid white;border-radius: 10px;padding: 2px;");
    m_lab_border->setHidden(true);

    m_b_isMenu = true;
}

AppItemWidget::~AppItemWidget()
{
    if(m_lab_caseTitle)
        delete m_lab_caseTitle;
    if(m_lab_icon)
        delete m_lab_icon;
    if(m_lab_border)
        delete m_lab_border;
}

void AppItemWidget::setText(QString text)
{
    QFontMetrics qfm(m_lab_caseTitle->font());
    m_lab_caseTitle->setText(qfm.elidedText(text,Qt::ElideRight,width() - 30));
}

void AppItemWidget::setRowNo(int rowNo)
{
    m_i_rowNo = rowNo;
}

void AppItemWidget::flush(int currentNo)
{
    if(currentNo == m_i_rowNo)
        m_lab_border->setHidden(false);
    else
        m_lab_border->setHidden(true);
}

//is Menu or Command
void AppItemWidget::setIsMenu(bool isMenu)
{
    m_b_isMenu = isMenu;
    if(m_b_isMenu)
        m_lab_icon->setHidden(false);
    else
        m_lab_icon->setHidden(true);
}

bool AppItemWidget::getIsMenu()
{
    return m_b_isMenu;
}
