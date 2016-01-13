#ifndef CHOICESET_H
#define CHOICESET_H

#include "UI/Common/AppBase.h"
#include "UI/Common/ScrollBar.h"
#include "UI/Common/AppListWidget.h"

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
    explicit Choiceset(AppListInterface * pList, QWidget *parent = 0);
    ~Choiceset();

protected:
    virtual void showEvent(QShowEvent * e);

signals:
    void menuClicked(int code, int choiceID);

public slots:
    void listWidgetDoubleClickedSlots(QModelIndex);
    void timeHideOutSlots();
    void menuClickedSlots(int code, int choiceID);

private:
    void addNewMenu(int choiceID, std::string menuName);
    void setChoicesetName(QString title);
    void setTimeOut(int duration);

private:
    AppListWidget *m_listWidget;

    QVector <SChoiceMenu > m_vec_choiceMenu;
    QVector <bool > m_vec_isMenu;
	QTimer *m_timerHide;

    void initLayout();
    void flushListWidget();
};

#endif // CHOICESET_H
