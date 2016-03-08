#ifndef MAINMENUE_H
#define MAINMENUE_H

#include <QEvent>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QStackedWidget>
#include <QHash>

#include "BaseWidght.h"
#include "AppData/AppListInterface.h"
#include "AppData/AppDataInterface.h"
#include "VideoStream/VideoStream.h"
typedef enum
{
    ID_MENU_MF=0,
    ID_MENU_TEL,
    ID_MENU_MSG,
    ID_MENU_CD,
    ID_MENU_LIST,
    ID_MENU_MAX
}ID_MENU_TYPE;

class MainMenu : public BaseWidght
{
    Q_OBJECT
public:
    explicit MainMenu(AppListInterface * pList, QWidget *parent = 0);
    ~MainMenu();
    QWidget *CenterWidget();

    void SetTitle(QString title);


    void StartVideoStream(const char* url);
    void StopVideoStream();

    virtual void keyPressEvent(QKeyEvent *e);

public slots:
    void ShowDateTime();
    void onMenuButtonClick();
    void menuBtnClickedSlots(QString btnText);


private:

signals:
    void menuBtnClicked(QString);

private:
    VideoStream* videoWidget;
    QLabel *labelTitle;
    QLabel *labelTime;
    QTimer *m_timer;
    BaseWidght *widgetLeftMenu;
    BaseWidght *widgetCenter;
    QPushButton *btnUpScroll;
    QPushButton *btnMenuList[ID_MENU_MAX];

    QStackedWidget *stackWidget;

    AppListInterface * m_pList;
};

#endif // MAINMENUE_H
