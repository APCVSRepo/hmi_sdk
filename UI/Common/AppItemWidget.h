#ifndef APPITEMWIDGET_H
#define APPITEMWIDGET_H

#include "UI/UIInclude.h"


class AppItemWidget : public QWidget
{
    Q_OBJECT
public:
    AppItemWidget(int w,int h,QWidget *parent = 0);
    ~AppItemWidget();

    // 显示app图标
    void SetIcon(QString strIconPath);
    void setText(QString text);

    void setStatus(bool status);

    void setIsMenu(bool isMenu);
    bool getIsMenu();

    void setFixedHeight(int h);

private slots:

private:

    QLabel *m_lab_caseTitle;
    QLabel *m_lab_icon;
    QLabel *m_lab_border;
    // 显示app图标
    QLabel *m_pAppIconLab;

    bool m_b_isMenu;
};


#endif // APPITEMWIDGET_H
