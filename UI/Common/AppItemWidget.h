#ifndef APPITEMWIDGET_H
#define APPITEMWIDGET_H

#include "UI/AppInclude.h"


class AppItemWidget : public QWidget
{
    Q_OBJECT

public:
    AppItemWidget(int w,int h,QWidget *parent = 0);
    ~AppItemWidget();

    void setText(QString text);
    void setRowNo(int rowNo);
    void flush(int currentNo);

    void setIsMenu(bool isMenu);
    bool getIsMenu();

private slots:


private:

    QLabel *m_lab_caseTitle;
    QLabel *m_lab_icon;
    QLabel *m_lab_border;

    int m_i_rowNo;

    bool m_b_isMenu;

};


#endif // APPITEMWIDGET_H
