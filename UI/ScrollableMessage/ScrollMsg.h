#ifndef CSCROLLMSG_H
#define CSCROLLMSG_H

#include "UI/Common/PopBase.h"
#include "QTextEdit"
#include "UI/Common/Button.h"

class CPopBase;
class CScrollMsg : public CPopBase
{
    Q_OBJECT
public:
    explicit CScrollMsg(QWidget *parent = 0);
    ~CScrollMsg();

    void execShow();
    void testShow();

signals:
    //void softButtonClicked(int btID, int mode);
    void scrollMsgAbort(int scrollMsgId, int reason);

public slots:
    void upClickedSlots();
    void downClickedSlots();
    void timeoutSlots();
    void onSpaceClicedSlots();
    void onButtonOneClickedSlots(int btID);
    void onButtonTwoClickedSlots(int btID);
    void onButtonThrClickedSlots(int btID);
    void onButtonFouClickedSlots(int btID);
    void onButtonOneClickedLongSlots(int btID);
    void onButtonTwoClickedLongSlots(int btID);
    void onButtonThrClickedLongSlots(int btID);
    void onButtonFouClickedLongSlots(int btID);
    void scrollMsgAbortSlots(int scrollMsgID, int reason);
private:
    void setMessage(QString msg);
    void setBtnText(int btnIdx, QString text, bool highLight);
    void setBtnID(int btnIdx, int id);
    void setTimeOut(int duration);

private:
    QTextEdit *m_editText;
    CButton *m_btn_up;
    CButton *m_btn_down;

    CButton    *m_btnSoft1;
    CButton    *m_btnSoft2;
    CButton    *m_btnSoft3;
    CButton    *m_btnSoft4;

    int m_i_scrollMsgID;
    void InitLayout();
};

#endif // CSCROLLMSG_H
