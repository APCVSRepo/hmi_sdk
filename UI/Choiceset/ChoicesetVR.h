#ifndef CCHOICESETVR_H
#define CCHOICESETVR_H

#include "QLabel"
#include "UI/Common/Button.h"
#include "Common/PopBase.h"
#define PERFORMINTERACTION_TIMEOUT      10
#define PERFORMINTERACTION_CHOICE       0
class CPopBase;
class CChoicesetVR : public CPopBase
{
    Q_OBJECT
public:
    explicit CChoicesetVR(QWidget *parent = 0);
    ~CChoicesetVR();

    void execShow();
    void testShow();

signals:
    void VRmenuClicked(int code, int id, int choiceID);

public slots:
    void timeoutSlots();
    void label1ClickedSlots(int id);
    void label2ClickedSlots(int id);
    void label3ClickedSlots(int id);
    void label4ClickedSlots(int id);
    void VRmenuClickedSlots(int code, int performInteractionID, int choiceID);

private:
    void setChoicesetVRText(int textIdx, QString text);
    void setChoicesetVRID(int btnIdx, int id);
    void setTimeOut(int duration);
    void setInteractionID(int id);

private:
    QLabel *m_labelVRIcon;
    QLabel *m_labelText;
    CButton *m_labelSet1;
    CButton *m_labelSet2;
    CButton *m_labelSet3;
    CButton *m_labelSet4;

    int m_i_interactionID;
    int m_i_defaultID;

    void InitLayout();
};

#endif // CCHOICESETVR_H
