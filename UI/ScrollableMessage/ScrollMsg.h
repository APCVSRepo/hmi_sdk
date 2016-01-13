#ifndef CSCROLLMSG_H
#define CSCROLLMSG_H

#include "UI/Common/AppBase.h"
#include "QTextEdit"
#include "UI/Common/Button.h"

class CScrollMsg : public AppBase
{
    Q_OBJECT
public:
    explicit CScrollMsg(AppListInterface * pList, QWidget *parent = 0);
    ~CScrollMsg();

protected:
    virtual void showEvent(QShowEvent * e);

signals:
    //void softButtonClicked(int btID, int mode);
    void scrollMsgAbort(int reason);
    void onSpaceCliced();

public slots:

    void timeoutSlots();
    void onSpaceClicedSlots();
    void onButtonClickedSlots(int btID);
    void onButtonClickedLongSlots(int btID);
    void scrollMsgAbortSlots(int reason);

private:
    void setMessage(QString msg);
    void setBtnText(int btnIdx, QString text, bool highLight);
    void setBtnID(int btnIdx, int id);
    void setTimeOut(int duration);

private:
    QTextEdit *m_editText;

    CButton    *m_btnSoft[4];
    QTimer m_timer;

    void InitLayout();
};

#endif // CSCROLLMSG_H
