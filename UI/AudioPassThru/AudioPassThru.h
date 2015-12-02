#ifndef CAUDIOPASSTHRU_H
#define CAUDIOPASSTHRU_H

#include "UI/Common/PopBase.h"
#include <QLabel>
#include "UI/Common/Button.h"

#define PERFORMAUDIOPASSTHRU_TIMEOUT    0
#define PERFORMAUDIOPASSTHRU_RETYP      7
#define PERFORMAUDIOPASSTHRU_DONE       0
#define PERFORMAUDIOPASSTHRU_CANCEL     5
class CPopBase;
class CAudioPassThru : public CPopBase
{
    Q_OBJECT
public:
    explicit CAudioPassThru(QWidget *parent = 0);
    ~CAudioPassThru();

    void execShow();
    void testShow();

signals:
    void audioPassThruHide(int, int);

public slots:
    void timeoutSlots();
    void onButtonClickedSlots(int);
    void audioPassThruHideSlots(int audioPassThruId, int code);

private:
    void setAudioText(int textIdx, QString text);
    void setAudioPassThruID(int id);
    void setTimeOut(int duration);
    void setAudioPassThruDisplayText1(QString text);
    void setAudioPassThruDisplayText2(QString text);

private:
    QLabel *m_labelAppName;
    QLabel *m_labelText1;
    QLabel *m_labelText2;
    QLabel *m_labelVRIcon;
    CButton *m_btn1;
    CButton *m_btn2;
    CButton *m_btn3;
    CButton *m_btn4;

    int m_i_performaudiopassthruID;
    void InitLayout();
    void setBtnText(int btnIdx, QString text);
    void setBtnID(int btnIdx, int id);
};

#endif // CAUDIOPASSTHRU_H
