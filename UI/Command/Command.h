#ifndef COMMAND_H
#define COMMAND_H

#include "UI/Common/AppBase.h"
#include "UI/Common/ScrollBar.h"
#include "UI/Common/AppItemWidget.h"

typedef struct commandID
{
    QString str_command;
    int i_cmdID;
}SCmdID;

class Command : public AppBase
{
    Q_OBJECT
public:
    explicit Command(QWidget *parent = 0);
    ~Command();

    void execShow(AppDataInterface* pAppInterface);

signals:
    void returnShow();
    void exitApp();
    void commandClick(int cmdID);

public slots:
    void listWidgetClickedSlots(QModelIndex);
    void listWidgetDoubleClickedSlots(QModelIndex);
    void upArrowSlots();
    void downArrowSlots();
    void backBtnClickSlots();
    void moveBackSlots();

private:
    void clearAllCommand();
    void addNewMenu(QString commandName, int menuID);
    void addNewCommand(QString commandName, int cmdID);
    void addSubCommand(QString menu, int menuID, QString subCommand, int cmdID);
    void delSubCommand(QString menu, QString subCommand);
    void delMenuCommand(QString commandName);

private:
    void initLayout();

    void hideBackIcon();
    void showBackIcon();
    void addListItemWidget(QString text, bool isMenu);

    void flushAllItems(int currentNo);
    void flushListWidget();

    QPushButton m_btn_backIcon;
    ScrollBar m_scrollBar;
    QListWidget m_listWidget;

    QVector <QListWidgetItem *> m_vec_listItem;
    QVector <AppItemWidget *> m_vec_appItemWidget;
//    QMap <QString ,int> m_map_cmdID;
    QVector <SCmdID> m_vec_cmdID;
    QVector <bool > m_vec_isMenu;
    QVector < QVector <SCmdID> > m_vec_subStrList;

    int m_i_currentRow;
    int m_i_totalRow;

    int m_i_showRow;
    bool m_b_downUp;

    bool m_b_backIcon;
};

#endif // COMMAND_H
