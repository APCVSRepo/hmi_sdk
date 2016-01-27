#ifndef APPLISTWIDGET_H
#define APPLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include "ScrollBar.h"
#include "AppItemWidget.h"
#include <qvector.h>


class AppListWidget : public QListWidget
{
    Q_OBJECT
public:
    AppListWidget(int x,int y,int w,int h,QWidget *parent=NULL);
    ~AppListWidget();
    static QString cssString();
	void FlushAll();
    void AddListItemWidget(QString text,bool isMenu=true);
    void AddListItemWidget(QString text,QString strIconPath="",bool isMenu = true);
    void DelListItemWidget();
    void SetScrollParams(int page,int range);
    void ScrollToIndex(int index);
    void ItemSelect(int index);
    void SetItemText(int index,QString txt);
    void SetTextAlignment(Qt::Alignment align);
    void setFixedSize(int w, int h);
signals:
    void clicked(int itemIndex);
    void longclicked(int itemIndex);
public slots:
    void onItemClicked(QModelIndex );
    void onItemDoubleClicked(QModelIndex);
private:
    ScrollBar m_scrollWidget;
	QVector<AppItemWidget*> m_itemList;
    int      m_LineHeight;
};

#endif // APPLISTWIDGET_H
