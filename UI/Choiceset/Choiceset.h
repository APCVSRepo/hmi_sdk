#ifndef CHOICESET_H
#define CHOICESET_H

#include "UI/Common/AppBase.h"
#include "UI/Common/ScrollBar.h"
#include "UI/Common/AppItemWidget.h"

#define PERFORMINTERACTION_TIMEOUT      10
#define PERFORMINTERACTION_CHOICE       0

typedef struct ChoiceMenu
{
    int i_choiceID;
    std::string str_menuName;
}SChoiceMenu;

class Choiceset : public AppBase
{
    Q_OBJECT
public:
    explicit Choiceset(QWidget *parent = 0);
    ~Choiceset();

    void execShow(AppDataInterface* pAppInterface);
    void testShow();

signals:
    void menuClicked(int code, int id, int choiceID);

public slots:
    void listWidgetClickedSlots(QModelIndex);
    void listWidgetDoubleClickedSlots(QModelIndex);
    void upArrowSlots();
    void downArrowSlots();
    void moveBackSlots();
    void timeHideOutSlots();

private:
    void addNewMenu(int choiceID, std::string menuName);
    void setInteractionID(int id);
    void setChoicesetName(QString title);
    void setTimeOut(int duration);

private:
    ScrollBar m_scrollBar;
    QListWidget m_listWidget;

    QVector <QListWidgetItem *> m_vec_listItem;
    QVector <AppItemWidget *> m_vec_appItemWidget;
    QVector <SChoiceMenu > m_vec_choiceMenu;
    QVector <bool > m_vec_isMenu;

    int m_i_currentRow;
    int m_i_totalRow;

    int m_i_showRow;
    bool m_b_downUp;

    bool m_b_backIcon;

    int m_i_interactionID;
    QTimer *m_timerHide;

    void initLayout();
    void hideBackIcon();
    void showBackIcon();
    void addListItemWidget(QString text, bool isMenu);
    void flushAllItems(int currentNo);
    void flushListWidget();
};

#endif // CHOICESET_H
