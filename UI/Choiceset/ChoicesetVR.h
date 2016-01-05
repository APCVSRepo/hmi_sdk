#ifndef CCHOICESETVR_H
#define CCHOICESETVR_H

#include "QLabel"
#include "UI/Common/Button.h"
#include "Common/AppBase.h"
#define PERFORMINTERACTION_TIMEOUT      10
#define PERFORMINTERACTION_CHOICE       0
class CPopBase;
class CChoicesetVR : public AppBase
{
    Q_OBJECT
public:
    explicit CChoicesetVR(AppListInterface * pList, QWidget *parent = 0);
    ~CChoicesetVR();

protected:
    virtual void showEvent(QShowEvent * e);

signals:
    void VRmenuClicked(int code, int choiceID);

public slots:
    void timeoutSlots();
    void labelClickedSlots(int id);
    void VRmenuClickedSlots(int code, int choiceID);

private:
    void setChoicesetVRText(int textIdx, QString text);
    void setChoicesetVRID(int btnIdx, int id);
    void setTimeOut(int duration);

private:
    QLabel *m_labelVRIcon;
    QLabel *m_labelText;
    CButton *m_labelSet1;
    CButton *m_labelSet2;
    CButton *m_labelSet3;
    CButton *m_labelSet4;
    QTimer m_timer;

    int m_i_defaultID;

    void InitLayout();
};

#endif // CCHOICESETVR_H
