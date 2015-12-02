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
typedef enum
{
    ID_MENU_MF=0,
    ID_MENU_TEL,
    ID_MENU_MSG,
    ID_MENU_CD,
    ID_MENU_LIST,
    ID_MENU_MAX
}ID_MENU_TYPE;
class MainMenue : public BaseWidght
{
    Q_OBJECT
public:
    explicit MainMenue(QWidget *parent = 0);
    ~MainMenue();
    void InserWidget(int id,QWidget *wdiget);
    void SetCurWidget(int id,bool history=true);
    void SetTitle(QString title);
    QWidget *CenterWidght();
    void ReceiveJson(int id,Json::Value json);
    void ExitWidget(int id);

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


};

#endif // MAINMENUE_H
