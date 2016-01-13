#include "AppListWidget.h"
#include "AppItemWidget.h"

AppListWidget::AppListWidget(int x,int y,int w,int h,QWidget *parent):QListWidget(parent)
{
    setGeometry(x,y,w,h);
#ifndef ANDROID
    QPalette pll = palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
    setPalette(pll);
#endif
    setStyleSheet(cssString());
    setFrameShape(QFrame::NoFrame); //设置无边框
    setFocusPolicy(Qt::NoFocus); //去除选中虚线框
    setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    setVerticalScrollBar(&m_scrollWidget);
    m_scrollWidget.init(h-30);
    SetScrollParams(4,4);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(onItemClicked(QModelIndex)));
}

void AppListWidget::FlushAll()
{
	ItemSelect(0);
}

void AppListWidget::AddListItemWidget(QString text,bool isMenu)
{
    QListWidgetItem *item = new QListWidgetItem;
    int w=width()-35;
    int h=m_LineHeight;
    item->setSizeHint(QSize(w,h));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsDragEnabled);//不响应突出
	addItem(item);
    AppItemWidget *itemWidget = new AppItemWidget(w,h,this);
    // insertItem(index,itemWidget);
    setItemWidget(item,itemWidget);

    itemWidget->setText(text);
    itemWidget->setIsMenu(isMenu);
	m_itemList.push_back(itemWidget);
}

void AppListWidget::DelListItemWidget()
{
    while(count()!=0){
        QListWidgetItem *itemWidget =this->item(0);
        this->removeItemWidget(itemWidget);
        delete itemWidget;
    }
	m_itemList.clear();
}

void AppListWidget::SetScrollParams(int page, int range)
{
    if(page<=0 || range<=0)
        return;
    m_scrollWidget.setPageStep(page);
    m_scrollWidget.setRange(0,range-1);
    m_LineHeight=(height()-10)/page;
    for (int i = 0; i<m_itemList.count(); i++){
        AppItemWidget *item = m_itemList.at(i);
        item->setFixedHeight(m_LineHeight);
    }
}

void AppListWidget::ScrollToIndex(int index)
{
    m_scrollWidget.setValue(index);
}

QString AppListWidget::cssString()
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

void AppListWidget::onItemClicked(QModelIndex index)
{
    ItemSelect(index.row());
    ScrollToIndex(index.row());
}

void AppListWidget::ItemSelect(int index)
{
	for (int i = 0; i<m_itemList.count(); i++){
		AppItemWidget *item = m_itemList.at(i);
        item->setStatus(!(index==i));
    }
}

void AppListWidget::SetItemText(int index, QString txt)
{
	if (index <= 0 || index >= m_itemList.count())
		return;
	AppItemWidget *item = m_itemList.at(index);
    if(item)
        item->setText(txt);
    else
        LOGI("ITEM SS null");
}

void AppListWidget::setFixedSize(int w, int h)
{
    QListWidget::setFixedSize(w,h);
    m_scrollWidget.init(h-30);
}

AppListWidget::~AppListWidget()
{
	DelListItemWidget();
}

