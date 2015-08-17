#ifndef APPBASE_H
#define APPBASE_H

#include "UI/AppInclude.h"
#include "../../AppData/AppDataInterface.h"

class AppBase : public QWidget
{
    Q_OBJECT
public:
    explicit AppBase(QWidget *parent = 0);
    ~AppBase();

    virtual void execShow(AppDataInterface* pAppInterface);

    QLabel m_lab_title;
    QLabel m_lab_time;
    QTimer *m_timer;

    QPushButton m_btn_FM;
    QPushButton m_btn_Tel;
    QPushButton m_btn_Msg;
    QPushButton m_btn_CD;
    QPushButton m_btn_List;

    QVBoxLayout *btnLayout;
    QHBoxLayout *menuLayout;

    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void moveBack();
    void onButtonClicked(int btnID);
    void onListClicked(int listID);
    void onSpaceClicked();

    void menuBtnClicked(QString);
public slots:
    void GetDateTime();

    void btnFMClickedSlots();
    void btnTelClickedSlots();
    void btnMsgClickedSlots();
    void btnCDClickedSlots();
    void btnListClickedSlots();

protected:
    Json::Value m_jsonData;
private:
    int m_i_leftRight;

    int m_i_count;

};

#endif // APPBASE_H
