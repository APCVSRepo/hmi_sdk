#ifndef CAUDIOPASSTHRU_H
#define CAUDIOPASSTHRU_H

#include "UI/Common/AppBase.h"
#include <QLabel>
#include "UI/Common/Button.h"

#define PERFORMAUDIOPASSTHRU_TIMEOUT    0
#define PERFORMAUDIOPASSTHRU_RETYP      7
#define PERFORMAUDIOPASSTHRU_DONE       0
#define PERFORMAUDIOPASSTHRU_CANCEL     5

class CAudioPassThru : public AppBase
{
    Q_OBJECT
public:
    explicit CAudioPassThru(AppListInterface * pList, QWidget *parent = 0);
    ~CAudioPassThru();

signals:
    void audioPassThruHide(int);
    void onSpaceCliced();

protected:
    virtual void showEvent(QShowEvent * e);

public slots:
    void timeoutSlots();
    void onButtonClickedSlots(int);
    void audioPassThruHideSlots(int code);

private:
    void InitLayout();
    void setAudioText(int textIdx, QString text);
    void setTimeOut(int duration);
    void setAudioPassThruDisplayText1(QString text);
    void setAudioPassThruDisplayText2(QString text);
    void setBtnText(int btnIdx, QString text);
    void setBtnID(int btnIdx, int id);

private:
    QLabel *m_labelAppName;
    QLabel *m_labelText1;
    QLabel *m_labelText2;
    QLabel *m_labelVRIcon;
    CButton *m_btn1;
    CButton *m_btn2;
    CButton *m_btn3;
    CButton *m_btn4;

    QTimer m_timer;
};

#endif // CAUDIOPASSTHRU_H
