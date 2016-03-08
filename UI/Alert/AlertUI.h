#ifndef CALERTUI_H
#define CALERTUI_H

#include "Common/AppBase.h"
#include "QLabel"
#include "Common/Button.h"

class CAlertUI : public AppBase
{
    Q_OBJECT
public:
    explicit CAlertUI(AppListInterface * pData, QWidget *parent = 0);
    ~CAlertUI();

protected:
    virtual void showEvent(QShowEvent * e);

signals:
    void alertAbort(int reason);
    void softButtonClicked(int btID, int mode);
    void onSpaceCliced();

public slots:
    void timeoutSlots();
    void onSpaceClicedSlots();
    void onButtonClickedSlots(int btID);
    void onButtonClickedLongSlots(int btID);
    void alertAbortSlots(int reason);

private:
    void setAlertText(int textIdx, QString text);
    void setBtnText(int btnIdx, QString text, bool highLight,int iSoftBtnId = 0);
    void setTimeOut(int duration);

private:
    QLabel *m_pLabelText[3];
//    QLabel *m_labelText1;
//    QLabel *m_labelText2;
//    QLabel *m_labelText3;
    CButton *m_btnSoft1;
    CButton *m_btnSoft2;
    CButton *m_btnSoft3;
    CButton *m_btnSoft4;

    QVBoxLayout *mLayout;
    QTimer m_timer;

    void InitLayout();
    void updateLayout();
};

#endif // CALERTUI_H
