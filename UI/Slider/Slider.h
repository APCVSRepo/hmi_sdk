#ifndef SLIDER_H
#define SLIDER_H

#include <QWidget>
#include "QLabel"
#include "../Common/Button.h"
#include "Common/PopBase.h"
#define SLIDER_OK                       0
#define SLIDER_TIMEOUT                  10
#define SLIDER_ABORTED                  5
class CPopBase;
class Slider : public CPopBase
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = 0);
    
    void execShow();
    void testShow();


signals:
    void sliderClicked( int code, int sliderid, int sliderPosition);
    
public slots:
    void onMoveLeftSlot();
    void onMoveRightSlot();
    void onButtonSaveClicked();
    void onButtonCancelClicked();
    void timeoutSlots();
    void sliderClickedSlots( int code, int sliderid, int sliderPosition);

private:
    void setSliderID(int id);
    void setSliderTitle(QString text);
    void setSliderStrings(std::vector<std::string> vec_strSliter, int iPos = 0);
    void setTimeOut(int duration);
private:
    QLabel *m_labelText1;
    QLabel *m_labelText2;
    QLabel *m_labelText3;
    CButton *m_btnSoft1;
    CButton *m_btnSoft2;
    CButton *m_btnSoft3;
    CButton *m_btnSoft4;

    int m_iPos;
    int m_i_sliderID;
    QString m_EditText;
    QVector <QString> m_Strings;
    void InitLayout();
    void updateScreen();
    bool setPosition(int iPos);
};

#endif // SLIDER_H
