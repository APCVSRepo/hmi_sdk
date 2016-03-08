#ifndef CSCROLLMSG_H
#define CSCROLLMSG_H

#include "UI/Common/AppBase.h"
#include <QTextEdit>
#include "UI/Common/Button.h"
#include "Common/AppListWidget.h"

class CScrollMsg : public AppBase
{
    Q_OBJECT
public:
    explicit CScrollMsg(AppListInterface * pList, QWidget *parent = 0);
    ~CScrollMsg();
    typedef struct
    {
        QString btnText;
        int     btnId;
        bool    isLighted;
    }SoftButton;

protected:
    virtual void showEvent(QShowEvent * e);

signals:
    //void softButtonClicked(int btID, int mode);
    void scrollMsgAbort(int reason);
    void onSpaceCliced();

public slots:

    void timeoutSlots();
    void onSpaceClicedSlots();
    void onButtonClickedSlots(int btID);
    void onButtonClickedLongSlots(int btID);
    void scrollMsgAbortSlots(int reason);
    void onItemClicked(int index);
    void onItemLongClicked(int index);

private:
    void setMessage(QString msg);
    void addSoftButton(int btnId, QString text, bool highLight);
    void setTimeOut(int duration);
    void setButtonStyle(int index,int btnId,QString text,bool highLighte);
    void delLayout(QLayout *layout);
private:
    QTextEdit *m_editText;
    AppListWidget *m_listWidget;
    CButton   *m_btnSoft[4];
    QTimer m_timer;
    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_pListLayout;
    QList<SoftButton> m_listButton;

    void InitLayout();
    void UpdateLayout();
    void ChangeLayout(int flag);//0:默认，1：列表
};

#endif // CSCROLLMSG_H
