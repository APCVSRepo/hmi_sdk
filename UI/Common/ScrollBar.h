#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "UI/AppInclude.h"
#include <QScrollBar>

class ScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit ScrollBar(QWidget *parent = 0);
    ~ScrollBar();

    void init(int rowNo, int h);
    void flushScroll(int currentNo, int totalNum);

signals:
//    void upClicked();
//    void downClicked();

public slots:

private:


    int m_i_rowNo;
    int m_i_totalNum;
    int m_i_currentNo;

    int m_i_scrollW;
    int m_i_scrollH;
    int m_i_startX;
    int m_i_startY;

    int m_i_clickX;
    int m_i_clickY;

    QPushButton m_btn_upArrow;
    QLabel m_lab_scroll;
    QPushButton m_btn_downArrow;
};

#endif // SCROLLBAR_H
