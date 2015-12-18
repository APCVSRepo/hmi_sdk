#ifndef APPBASE_H
#define APPBASE_H

#include "MainMenu.h"
#include "UIInterface.h"

class AppBase : public QWidget
{
    Q_OBJECT
public:
    explicit AppBase(AppListInterface * pList, QWidget *parent = 0);
    ~AppBase();

    virtual void setBkgImage(const char *img);

    virtual void execShow();
    virtual void receiveJson(Json::Value json);

    virtual void showCurUI(int id){((MainMenu*)parent()->parent()->parent())->SetCurWidget(id);}
    virtual void setTitle(QString title){((MainMenu*)parent()->parent()->parent())->SetTitle(title);}
    virtual void goBack(){((MainMenu*)parent()->parent()->parent())->onMoveBack();}
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void onButtonClicked(int btnID);
    void onListClicked(int listID);
    void onSpaceClicked();

    void menuBtnClicked(QString);

protected:
    AppListInterface * m_pList;
    Json::Value m_jsonData;

private:
    int m_i_leftRight;

    int m_i_count;

};

#endif // APPBASE_H
