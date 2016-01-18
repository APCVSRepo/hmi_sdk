#ifndef SLIDER_H
#define SLIDER_H

#include <QWidget>
#include "QLabel"
#include "../Common/Button.h"
#include "Common/AppBase.h"
#define SLIDER_OK                       0
#define SLIDER_TIMEOUT                  10
#define SLIDER_ABORTED                  5

class Slider : public AppBase
{
    Q_OBJECT
public:
    explicit Slider(AppListInterface * pList, QWidget *parent = 0);
    ~Slider();
    
protected:
    virtual void showEvent(QShowEvent * e);


signals:
    void sliderClicked( int code, int sliderPosition);
    void onSpaceCliced();

public slots:
    void onMoveLeftSlot();
    void onMoveRightSlot();
    void onButtonSaveClicked();
    void onButtonCancelClicked();
    void timeoutSlots();
    void sliderClickedSlots( int code, int sliderPosition);

private:
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
    QTimer m_timer;

    int m_iPos;
    QString m_EditText;
    QVector <QString> m_Strings;
    void InitLayout();
    void updateScreen();
    bool setPosition(int iPos);
};

#endif // SLIDER_H
