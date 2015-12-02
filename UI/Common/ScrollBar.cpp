#include "ScrollBar.h"

ScrollBar::ScrollBar(QWidget *parent):QScrollBar(parent)
{
    this->setStyleSheet(QString::fromUtf8("\
QScrollBar::vertical {\
background:rgb(63,70,87);\
    border:0px solid grey;\
    width: 30px;}\
QScrollBar::handle:vertical {\
    background: rgb(108,113,125);\
    border: 3px solid grey;\
    border-radius:5px;\
    min-height: 20px;\
    width:20px;\
}\
QScrollBar::add-line:vertical {\
    height: 0px;\
    subcontrol-position: bottom;\
    subcontrol-origin:margin;\
border-image:url(:/images/downarrow.png);\
}\
QScrollBar::sub-line:vertical {\
    height: 0px;\
    subcontrol-position: top;\
    subcontrol-origin:margin;\
border-image:url(:/images/uparrow.png);\
}\
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\
    background: none;\
}\
QScrollArea {\
    border:0;\
    background:rgb(63,70,87);\
}\
"));

//QScrollBar::up-arrow:vertical{ subcontrol-origin: margin; \
//background-image:url(:/images/uparrow.png);\
//height:13px;}\
//QScrollBar::down-arrow:vertical{ background-image:url(:/images/downarrow.png); }
}

ScrollBar::~ScrollBar()
{

}

void ScrollBar::init(int rowNo, int h)
{

    this->setOrientation(Qt::Vertical);
    this->setPageStep(rowNo);
   this->setFixedHeight(h);
}

void ScrollBar::flushScroll(int currentNo, int totalNum)
{
    this->setRange(0,totalNum-1);
    this->setValue(currentNo-1);
}


