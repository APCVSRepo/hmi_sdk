#include "AppItemWidget.h"
extern Config g_config;
AppItemWidget::AppItemWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(g_config.m_i_itemW,g_config.m_i_itemH);

  //  this->setStyleSheet("background-color: transparent;");

    m_lab_caseTitle = new QLabel;
    m_lab_icon = new QLabel;
    m_lab_border = new QLabel;

    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addStretch(10);
    mLayout->addWidget(m_lab_caseTitle,80,Qt::AlignLeft);
    mLayout->addWidget(m_lab_icon,10,Qt::AlignRight);


    m_lab_caseTitle->setStyleSheet("font: 75 36pt \"Liberation Serif\";color:rgb(255,255,255);border: 0px");

    QPixmap pixmap(":/images/rightarrow.png");
    QPixmap fitpixmap=pixmap.scaled(30,30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_lab_icon->setPixmap(fitpixmap);

    m_lab_border->setParent(this);
    m_lab_border->setGeometry(5,5,g_config.m_i_itemW-10,g_config.m_i_itemH-10);
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
    m_lab_caseTitle->setText(text);
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
