#ifndef CALERTUI_H
#define CALERTUI_H

#include "UI/Common/PopBase.h"
#include "QLabel"
#include "UI/Common/Button.h"

class CAlertUI : public CPopBase
{
    Q_OBJECT
public:
    explicit CAlertUI(CPopBase *parent = 0);
    ~CAlertUI();

public:
    void execShow(AppDataInterface* pAppInterface);


signals:
    void alertAbort(int alertID, int reason);
    void softButtonClicked(int btID, int mode);

public slots:
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

private:
    void setAlertText(int textIdx, QString text);
    void setBtnText(int btnIdx, QString text, bool highLight);
    void setTimeOut(int duration);

private:
    QLabel *m_labelText1;
    QLabel *m_labelText2;
    QLabel *m_labelText3;
    CButton *m_btnSoft1;
    CButton *m_btnSoft2;
    CButton *m_btnSoft3;
    CButton *m_btnSoft4;

    QVBoxLayout *mLayout;

    void InitLayout();
    void updateLayout();

    int m_i_alertID;
};

#endif // CALERTUI_H
