#ifndef POPBASE_H
#define POPBASE_H

#include <QWidget>
#include "QTimer"
#include "QLabel"
#include "Background.h"
#include "AppData/AppDataInterface.h"
#include "BaseWidght.h"
#include "AppData/DataManager.h"
#include "MainMenue.h"
#include "UIInterface.h"
#include <QDialog>

class CPopBase : public QWidget
{
    Q_OBJECT
public:
    explicit CPopBase(QWidget *parent = 0);

public:
    virtual void execShow();
    virtual void showCurUI(int id)
    {
		((MainMenue*)parent()->parent()->parent())->SetCurWidget(id);
    }
    virtual void goBack()
    {
        ((MainMenue*)parent()->parent()->parent())->onMoveBack();
    }

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
private:
//    Background m_labelBackspace;
};

#endif // POPBASE_H
