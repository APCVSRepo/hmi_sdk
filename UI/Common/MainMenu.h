#ifndef MAINMENUE_H
#define MAINMENUE_H

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
    void InserWidget(int id,QWidget *wdiget);
    void SetCurWidget(int id,bool history=true);
    void SetTitle(QString title);
    QWidget *CenterWidght();
    void ReceiveJson(int id,Json::Value json);
    void ExitWidget(int id);

    void StartVideoStream(const char* url);
    void StopVideoStream();

    virtual void keyPressEvent(QKeyEvent *e);
private:

signals:
    void menuBtnClicked(QString);
public slots:
    void GetDateTime();
    void onMenuButtonClick();
    void menuBtnClickedSlots(QString btnText);
    void onMoveBack();
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
    QHash<int,int> stackKeys;
    QList<int>     stackHistory;
    int            closeCount;

    AppListInterface * m_pList;
};

#endif // MAINMENUE_H
