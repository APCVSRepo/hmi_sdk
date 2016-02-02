#ifndef CAPPBUTTON_H
#define CAPPBUTTON_H

#include <QWidget>
#include "Common/MenuButton.h"

class CAppButton : public MenuButton
{
    Q_OBJECT
public:
    CAppButton(QWidget *pParent=NULL);
    ~CAppButton();
    virtual void setIcon(const QString on,const QString off,bool bPaint = false);
    void setFuncId(int id);
    int  getFuncId();
signals:
    void clickedWitchFuncId(int funcId);
public slots:

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    int    m_FuncId;
};

#endif // CAPPBUTTON_H
