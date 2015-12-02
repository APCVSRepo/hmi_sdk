#ifndef APPBASE_H
#define APPBASE_H

#include "AppData/DataManager.h"
#include "MainMenue.h"
#include "UIInterface.h"

class AppBase : public QWidget
{
    Q_OBJECT
public:
    explicit AppBase(QWidget *parent = 0);
    ~AppBase();

    virtual void setBkgImage(char *img);

    virtual void execShow();
    virtual void receiveJson(Json::Value json);

    virtual void showCurUI(int id){((MainMenue*)parent()->parent()->parent())->SetCurWidget(id);}
    virtual void setTitle(QString title){((MainMenue*)parent()->parent()->parent())->SetTitle(title);}
    virtual void goBack(){((MainMenue*)parent()->parent()->parent())->onMoveBack();}
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void onButtonClicked(int btnID);
    void onListClicked(int listID);
    void onSpaceClicked();

    void menuBtnClicked(QString);
public slots:
    virtual void moveBackSlots();
protected:
    Json::Value m_jsonData;
private:
    int m_i_leftRight;

    int m_i_count;

};

#endif // APPBASE_H
