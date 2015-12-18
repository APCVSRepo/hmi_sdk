#ifndef POPBASE_H
#define POPBASE_H

#include <QWidget>
#include "QTimer"
#include "QLabel"
#include "Background.h"
#include "AppData/AppListInterface.h"
#include "BaseWidght.h"
#include "MainMenu.h"
#include "UIInterface.h"
#include <QDialog>

class CPopBase : public QWidget
{
    Q_OBJECT
public:
    explicit CPopBase(AppListInterface * pList, QWidget *parent = 0);

public:
    virtual void execShow();
    virtual void showCurUI(int id)
    {
        ((MainMenu*)parent()->parent()->parent())->SetCurWidget(id);
    }
    virtual void goBack()
    {
        ((MainMenu*)parent()->parent()->parent())->onMoveBack();
    }

    void setBkgImage(const char *img);

    virtual void mouseReleaseEvent(QMouseEvent *event);

signals:
    void onButtonClicked(int btnID);
    void onListClicked(int listID);
    void onSpaceCliced();
    //void goBack();

public slots:
    
protected:
    QTimer *m_timer;
//    QLabel m_labelFrame;
//    QLabel m_labelBackground;
    Json::Value m_jsonData;
//    Background m_labelBackspace;
    AppListInterface * m_pList;
};

#endif // POPBASE_H
