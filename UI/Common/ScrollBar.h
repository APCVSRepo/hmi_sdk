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
    static QString cssString();
    void init(int rowNo, int h);
    void flushScroll(int currentNo, int totalNum);

signals:
//    void upClicked();
//    void downClicked();

public slots:

private:
};

#endif // SCROLLBAR_H
