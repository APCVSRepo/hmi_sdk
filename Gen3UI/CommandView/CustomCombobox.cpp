#include "CustomCombobox.h"

#define PAGEITEM 5
#define PAGEITEMWEIGHT 0.2

CustomCombobox::CustomCombobox(int iMaxHeight,bool bUp,QWidget *parent) : QListWidget(parent)
  ,m_iHeight(0),m_iStartX(0),m_iStartY(0),m_iWidth(0),m_iOldHoverItemIndex(-1)
{
    m_bUp = bUp;
    m_iMaxHeight = iMaxHeight / PAGEITEM * PAGEITEM;
    setFixedHeight(m_iMaxHeight);

#ifndef ANDROID
    QPalette pll = palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
    setPalette(pll);
#endif

    setStyleSheet(cssString());
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setVerticalScrollBar(&m_scrollWidget);
    m_scrollWidget.setStyleSheet(QString::fromUtf8("\
QScrollBar::vertical {background:none;border:0px solid grey;width: 30px;margin:30px 0px 30px 0px;border-image: url(:images/SliderBack.png);}\
QScrollBar::handle:vertical {background: rgb(255,113,125);border: 1px solid grey;border-radius:0px;min-height: 20px;width:30px;border-image: url(:images/Slider.png);}\
QScrollBar::add-line:vertical {height: 31px;subcontrol-origin:margin;}\
QScrollBar::sub-line:vertical {height: 31px;subcontrol-origin:margin;}\
QScrollBar::up-arrow:vertical {background:none;}\
QScrollBar::down-arrow:vertical {border-image: url(:images/DownArrowNormal.png);}\
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;border:0,0,0,0;}\
QScrollArea {border:0,0,0,0;background:rgb(63,70,87);}"));
    m_scrollWidget.init(m_iMaxHeight);
    //SetScrollParams(4,4);
    setFrameShape(QFrame::NoFrame);
    setFocusPolicy(Qt::NoFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setBackgroundRole(QPalette::Background);

    setMouseTracking(true);

    m_pTimer = new QTimer;
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(OnTimeOutSlot()));
    m_pTimer->start(50);
}

CustomCombobox::~CustomCombobox()
{
    delete m_pTimer;
}

void CustomCombobox::ClearAllItem()
{
    QListWidgetItem *pTempItem = nullptr;
    while(count() != 0)
    {
        pTempItem = item(0);
        removeItemWidget(pTempItem);
        delete pTempItem;
    }

    for(int i = 0;i != m_itemList.size();++i)
    {
        delete m_itemList[i];
    }
    m_itemList.clear();

    m_iHeight = 0;
    m_iOldHoverItemIndex = -1;
}

void CustomCombobox::AddListItem(QString strText,bool bMenu)
{
    QListWidgetItem *item = new QListWidgetItem;
    int w = width()-30;
    int h = m_iMaxHeight * PAGEITEMWEIGHT;
    item->setSizeHint(QSize(w,h));
    //item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsDragEnabled);
    addItem(item);
    CustomComboboxItem *itemWidget = new CustomComboboxItem(w,h,this);
    // insertItem(index,itemWidget);
    itemWidget->SetBkPic(":/images/CommandItemNormal.png",":/images/CommandItemPress.png",":/images/CommandItemNormal.png");
    setItemWidget(item,itemWidget);    
    itemWidget->setText(strText);
    itemWidget->setIsMenu(bMenu);
    m_itemList.push_back(itemWidget);

    //connect(itemWidget,SIGNAL(MouseOnItemSignal(CustomComboboxItem *)),this,SLOT(OnMouseMoveOnItem(CustomComboboxItem *)));

    m_iHeight += h;
    if(m_iHeight > m_iMaxHeight)
    {
        m_iHeight = m_iMaxHeight;
    }

    //SetPos(m_iStartX,m_iStartY,m_iWidth,m_iHeight);
}

void CustomCombobox::SetPos(int iStartX,int iStartY,int iWidth,int iHeight)
{
    if(m_iWidth == 0)
    {
        m_iStartX = iStartX;
        m_iStartY = iStartY;
        m_iWidth = iWidth;
        m_iHeight = iHeight;
    }

    if(m_bUp)
    {
        setGeometry(iStartX,iStartY - iHeight,iWidth,iHeight);
    }
    else
    {
        setGeometry(iStartX,iStartY,iWidth,iHeight);
    }    
}

void CustomCombobox::SetScrollParams(int page,int range)
{
    /*
    if(page<=0 || range<=0)
        return;
    m_scrollWidget.setPageStep(page);
    m_scrollWidget.setRange(0,range-1);
    m_LineHeight = (height()-10)/page;
    for (int i = 0; i<m_itemList.count(); i++){
        AppItemWidget *item = m_itemList.at(i);
        item->setFixedHeight(m_LineHeight);
    }
    */
}

void CustomCombobox::OnTimeOutSlot()
{
    int x = QCursor::pos().x();
    int y = QCursor::pos().y();
    if(x > m_iStartX && x < m_iStartX + m_iWidth && y > m_iStartY - m_iHeight && y < m_iStartY)
    {
        y -= m_iStartY - m_iHeight;
        int iIndex = y * PAGEITEM / m_iMaxHeight + m_scrollWidget.sliderPosition();
        if(m_iOldHoverItemIndex != iIndex)
        {
            if(m_iOldHoverItemIndex != -1)
            {
                m_itemList[m_iOldHoverItemIndex]->ChangeBkPic(CustomComboboxItem::NORMAL_STATE);
            }
            m_iOldHoverItemIndex = iIndex;
            m_itemList[iIndex]->ChangeBkPic(CustomComboboxItem::HOVER_STATE);
        }
    }
}

QString CustomCombobox::cssString()
{
#if 0
    return QString::fromUtf8("QListWidget{ \
                             border: none;\
                             color:white;\
                             background-color:transparent;\
                         }  \
                         QListWidget:item:hover{border: 0px;}\
                         QListWidget::item:selected { \
                              border:none;  \
                              background:none;\
                              border: 2px solid white;border-radius: 10px;padding: 2px;\
                         }  \
                         ");
#endif
#ifdef ANDROID
    return QString::fromUtf8("background-color:transparent;");
#else
    return QString::fromUtf8("QListWidget:item:hover{border:0px;}");
#endif
}

