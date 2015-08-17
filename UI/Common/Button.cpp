#include "Button.h"

CButton::CButton(QWidget *parent) :
    QWidget(parent),
    m_BtnStatus(BTN_STAT_UNKNOW),
    m_i_clickX(0),
    m_i_clickY(0)
{
    m_IconLabel.setParent(this);
    m_TextLabel.setParent(this);
}

/***********************************************************************************************************
 * Description:
 *          初始化按钮大小及背景图片, 默认显示常规背景。
 * Para:
 *          W: 按钮宽度
 *          H: 按钮高度
 *          normalIcon: 常规背景
 *          pressedIcon: 按下背景
 *          disableIcon: 操作不可背景
 * Auther:
 *         001 LUWANJIA 2015.04.22 新规
 ***********************************************************************************************************/
CButton::CButton(int W, int H, QString normalIcon, QString pressedIcon, QString disableIcon) :
    m_BtnStatus(BTN_STAT_UNKNOW),
    m_i_clickX(0),
    m_i_clickY(0),
    m_Id(0)
{
    setSize(W, H);
    setIconNormal(normalIcon, true);
    setIconPressed(pressedIcon, false);
    setIconDisable(disableIcon, false);
}

CButton::~CButton()
{

}

/***********************************************************************************************************
 * Description:
 *          设置按钮大小及背景图片, 默认显示常规背景。
 * Para:
 *          W: 按钮宽度
 *          H: 按钮高度
 *          normalIcon: 常规背景
 *          pressedIcon: 按下背景
 *          disableIcon: 操作不可背景
 * Auther:
 *         001 LUWANJIA 2015.04.22 新规
 ***********************************************************************************************************/
void CButton::initParameter(int W, int H, QString normalIcon, QString pressedIcon, QString disableIcon, QString text)
{
    setSize(W, H);
    setIconNormal(normalIcon, true);
    setIconPressed(pressedIcon, false);
    setIconDisable(disableIcon, false);
    setText(text);
}

void CButton::setId(int id)
{
    m_Id = id;
}

/***********************************************************************************************************
 * Description:
 *          设置按钮大小。
 * Para:
 *          W: 按钮宽度
 *          H: 按钮高度
 * Auther:
 *         001 LUWANJIA 2015.04.22 新规
 ***********************************************************************************************************/
void CButton::setSize(int W, int H)
{
    this->setMaximumSize(W, H);
    this->setMinimumSize(W, H);
    m_IconLabel.setMaximumSize(W, H);
    m_IconLabel.setMinimumSize(W, H);
    m_IconLabelExtra.setMaximumSize(W, H);
    m_IconLabelExtra.setMinimumSize(W, H);
}

/***********************************************************************************************************
 * Description:
 *          设置按钮常规背景。
 * Para:
 *          normalIcon: 图片路径
 *          flag: 标志，true：更新背景，false：不更新背景，仅设置路径
 * Auther:
 *         001 LUWANJIA 2015.04.22 新规
 ***********************************************************************************************************/
void CButton::setIconNormal(QString imagePath, bool flag)
{
    m_IconNormalPath = imagePath;
    if (flag)
    {
        changeToNormal();
    }
}

/***********************************************************************************************************
 * Description:
 *          设置按钮按下背景。
 * Para:
 *          normalIcon: 图片路径
 *          flag: 标志，true：更新背景，false：不更新背景，仅设置路径
 * Auther:
 *         001 LUWANJIA 2015.04.22 新规
 ***********************************************************************************************************/
void CButton::setIconPressed(QString imagePath, bool flag)
{
    m_IconPressedPath = imagePath;
    if (flag)
    {
        changeToPressed();
    }
}

/***********************************************************************************************************
 * Description:
 *          设置按钮操作不可背景。
 * Para:
 *          normalIcon: 图片路径
 *          flag: 标志，true：更新背景，false：不更新背景，仅设置路径
 * Auther:
 *         001 LUWANJIA 2015.04.22 新规
 ***********************************************************************************************************/
void CButton::setIconDisable(QString imagePath, bool flag)
{
    m_IconDisablePath = imagePath;
    if (flag)
    {
        changeToDisable();
    }
}

void CButton::setIconExtra(QString imagePath)
{
    m_IconExtraPath = imagePath;
    changeToExtra();
}

/***********************************************************************************************************
 * Description:
 *          切换是否显示顶层图片。
 * Para:
 *          flag：标志，true：显示，false:不显示。
 * Auther:
 *         001 LUWANJIA 2015.04.22 新规
 ***********************************************************************************************************/
void CButton::changeToExtra(bool flag)
{
    QImage image;
    if (flag)
    {
        image.load(m_IconExtraPath);
    }
    else
    {
        image.load("");
    }
    QPixmap pixmap(QPixmap::fromImage(image));
    QPixmap fitpixmap=pixmap.scaled(image.width(), image.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_IconLabelExtra.setPixmap(fitpixmap);
    m_IconLabelExtra.setParent(this);
    m_IconLabelExtra.setGeometry((this->width() - m_IconLabelExtra.width()) / 2, (this->height() - m_IconLabelExtra.height()) / 2, m_IconLabelExtra.width(), m_IconLabelExtra.height());
    m_IconLabelExtra.setWindowFlags(Qt::WindowStaysOnTopHint);
    m_IconLabelExtra.setAlignment(Qt::AlignCenter);
}

void CButton::setText(QString text, bool flag)
{
    m_Text = text;

    if (flag)
    {
        updateText();
    }
}

void CButton::setTextStyle(QString style, bool flag)
{
    m_Style = style;
    if (flag)
    {
        updateTextStyle();
    }
}

void CButton::updateText()
{
    m_TextLabel.setText(m_Text);
    m_TextLabel.setAlignment(Qt::AlignCenter);
    m_TextLabel.setGeometry((this->width() - m_TextLabel.width()) / 2, (this->height() - m_TextLabel.height()) / 2, m_TextLabel.width(), m_TextLabel.height());
    m_TextLabel.setAttribute(Qt::WA_TranslucentBackground, true);
}

void CButton::updateTextStyle()
{
    m_TextLabel.setStyleSheet(m_Style);
}

/***********************************************************************************************************
 * Description:
 *          切换至常规状态并更新背景。
 * Para:
 *          -
 * Auther:
 *         001 LUWANJIA 2015.04.22 新规
 ***********************************************************************************************************/
void CButton::changeToNormal()
{
    QPixmap pixmap(m_IconNormalPath);
    QPixmap fitpixmap=pixmap.scaled(m_IconLabel.width(), m_IconLabel.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_IconLabel.setPixmap(fitpixmap);
    m_BtnStatus = BTN_STAT_NORMAL;
}

/***********************************************************************************************************
 * Description:
 *          切换至按下状态并更新背景。
 * Para:
 *          -
 * Auther:
 *         001 LUWANJIA 2015.04.22 新规
 ***********************************************************************************************************/
void CButton::changeToPressed()
{
    QPixmap pixmap(m_IconPressedPath);
    QPixmap fitpixmap=pixmap.scaled(m_IconLabel.width(), m_IconLabel.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_IconLabel.setPixmap(fitpixmap);
    m_BtnStatus = BTN_STAT_PRESSED;
}

/***********************************************************************************************************
 * Description:
 *          切换至无效状态并更新背景。
 * Para:
 *          -
 * Auther:
 *         001 LUWANJIA 2015.04.22 新规
 ***********************************************************************************************************/
void CButton::changeToDisable()
{
    QPixmap pixmap(m_IconDisablePath);
    QPixmap fitpixmap=pixmap.scaled(m_IconLabel.width(), m_IconLabel.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_IconLabel.setPixmap(fitpixmap);
    m_BtnStatus = BTN_STAT_DISABLE;
}

bool CButton::isNormal()
{
    return m_BtnStatus == BTN_STAT_NORMAL ? true : false;
}

bool CButton::isPressed()
{
    return m_BtnStatus == BTN_STAT_PRESSED ? true : false;
}

bool CButton::isDisable()
{
    return m_BtnStatus == BTN_STAT_DISABLE ? true : false;
}

void CButton::mousePressEvent(QMouseEvent *e)
{
    m_i_clickX = e->x();
    m_i_clickY = e->y();
    //点击不触发事件，释放按钮才触发事件；
    QPixmap pixmap(m_IconPressedPath);
    QPixmap fitpixmap=pixmap.scaled(m_IconLabel.width(), m_IconLabel.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_IconLabel.setPixmap(fitpixmap);

    m_i_time = QTime::currentTime().secsTo(QTime(2000,1,1));
}

void CButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()== Qt::LeftButton)
    {
        setIconNormal(m_IconNormalPath);
        if((m_i_clickX == e->x()) && (m_i_clickY == e->y()))
        {
            if(m_i_time - (unsigned int)QTime::currentTime().secsTo(QTime(1970,1,1)) >=2 )
            {

                emit clickedLong();
                emit clickedLong(m_Id);
            }
            else
            {
                emit clicked();
                emit clicked(m_Id);
            }
            m_i_clickX = 0;
            m_i_clickY = 0;
        }
    }
}
void CButton::mouseMoveEvent(QMouseEvent *event)
{
    if(event->x() > this->width() || event->y() > this->height() || event->x() < 0 || event->y() < 0)
    {
        setIconNormal(m_IconNormalPath);
    }
}
