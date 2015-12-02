#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include "UI/UIManager.h"
#include "AppData/DataManager.h"

class SplashScreen : public QWidget
{
    Q_OBJECT
public:
    explicit SplashScreen(QWidget *parent = 0);
    ~SplashScreen();
    virtual void paintEvent(QPaintEvent *e);

    int m_screen_w;
    int m_screen_h;


signals:
    void finishUIInit();
public slots:
    void showMainUI();
    void onUIInit();
private:
    CUIManager *m_ui;
    DataManager *manager;
};

#endif // SPLASHSCREEN_H
