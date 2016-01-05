#include "Notify.h"
#include "UI/Config/Config.h"
#include "Common/AppBase.h"
#include <QTime>
#include <QApplication>
Notify::Notify(QWidget *parent) :
    AppBase(NULL, parent)
{
    InitLayout();
    connect(this, SIGNAL(onSpaceCliced()), this, SLOT(hide()));
}

void Notify::InitLayout()
{
    this->setFixedWidth(ui_app_width);

//    m_labelBackground.setParent(this);
//    m_labelBackground.setFixedWidth(ui_app_width);
//    m_labelFrame.setParent(this);
//    m_labelFrame.setFixedWidth(ui_app_width);

//    m_labelBackground.setStyleSheet("border: 1px solid black;border-radius:10px;background-color:rgb(36,43,61)");

    m_labelText.setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:rgb(255,255,254)");
    m_labelText.setAlignment(Qt::AlignCenter);
    m_labelText.setWordWrap(true);

    mLayout = new QVBoxLayout;
    mLayout->addWidget(&m_labelText, Qt::AlignCenter);

//    m_labelFrame.setLayout(mLayout);
    this->setLayout(mLayout);

}

void Notify::setText(QString text)
{
    m_labelText.setText(text);
    m_labelText.adjustSize();

//    m_labelFrame.setGeometry((this->width() - m_labelFrame.width()) / 2, 0.85 * this->height() - m_labelText.height() - 35, m_labelFrame.width(), m_labelText.height() + 35);
//    m_labelBackground.setGeometry((this->width() - m_labelBackground.width()) / 2, 0.85 * this->height() - m_labelText.height() - 35, m_labelBackground.width(), m_labelText.height() + 35);;
}

#include <QCoreApplication>
void Notify::waitSec(int s)
{
//#ifdef WIN32
//        Sleep(1000);
//#else
//        sleep(1);
//#endif

    QTime t;
    t.start();
    while(t.elapsed()<1000*s)
        QApplication::processEvents();
}

void Notify::showEvent(QShowEvent * e)
{
    this->setText("Searching...");

    waitSec(1);
    this->setText("No additional\nApps found.");

    waitSec(1);
    this->hide();
    LOGD("Notify goBack\n");
}
