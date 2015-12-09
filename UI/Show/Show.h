#ifndef SHOW_H
#define SHOW_H

#include "UI/Common/AppBase.h"
#include "UI/Common/ScrollBar.h"
#include "UI/Common/Button.h"

typedef struct softButton{
    bool b_isHighlighted;
    int i_softButtonID;
    std::string str_text;
    softButton()
    {
        b_isHighlighted = false;
        i_softButtonID = 0;
        str_text.clear();
    }
}SSoftButton;

class Show : public AppBase
{
    Q_OBJECT
public:
    explicit Show(AppListInterface * pList, QWidget *parent = 0);
    ~Show();

    void execShow();
    void setMediaColckTimer(Json::Value jsonObj);

signals:
    void moreClicked();

    void returnAppLink();

    //void softButtonClicked(int btID, int mode);

public slots:
    void upArrowSlots();
    void downArrowSlots();

    void onListSelect(const QModelIndex &index);

    void btnOneClickedSlots(int);
    void btnTwoClickedSlots(int);
    void btnThrClickedSlots(int);
    void btnFourClickedSlots();

    void btnThrClickedLongSlots(int btID);
    void btnTwoClickedLongSlots(int btID);
    void btnOneClickedLongSlots(int btID);

    void mediaClockSlots();

private:
    void setMainField1(bool isShow, QString text);
    void setMainField2(bool isShow, QString text);
    void setMainField3(bool isShow, QString text);
    void setMainField4(bool isShow, QString text);
    void setAlignment(int type);
    void setMediaTrack(bool isShow, QString text);
    void setMediaClock(bool isShow, QString text);
    void setSoftButtons(std::vector<SSoftButton> vec_softButtons);

    void initLayout();
    void addListItem();

    QLabel m_lab_icon;
    QListWidget m_listWidget;
    QLabel m_lab_mediaTrack;
    QLabel m_lab_mediaClock;

    ScrollBar m_scrollBar;

    CButton *m_btn_one;
    CButton *m_btn_two;
    CButton *m_btn_thr;
    CButton *m_btn_fou;

    QVector <QListWidgetItem *> m_vec_listItem;
    QVector <QLabel *> m_vec_listLabel;
    std::vector<SSoftButton> m_vec_softButtons;

    int m_i_totalNum;
    int m_i_currentNo;

    QTimer *m_timer_mediaClock;
    QTime nowMeidaClockTime;
    int m_i_startH;
    int m_i_startM;
    int m_i_startS;
    int m_i_endH;
    int m_i_endM;
    int m_i_endS;
    bool m_b_countup;

};

#endif // SHOW_H
