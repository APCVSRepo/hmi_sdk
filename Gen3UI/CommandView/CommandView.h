#ifndef COMMANDVIEW_H
#define COMMANDVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include "UI/Common/Button.h"
#include <QLabel>
#include "CustomCombobox.h"
#include "AppData/AppListInterface.h"


typedef struct CommandInformation
{
    std::string strCmd;
    int iParentId;
    // 如果是Menu则是MenuID，如果是Command则是CommandID
    int iId;
    bool bMenu;
    std::vector<CommandInformation> CmdVec;
}tagCmdInf;

class CCommandView : public QWidget
{
    Q_OBJECT
public:
    explicit CCommandView(AppListInterface *pList,QWidget *parent = 0);
    ~CCommandView();

protected:
    virtual void showEvent(QShowEvent * e);
signals:

public slots:
    void OnReturnBtnClicked();
private:
    void AddCommand(int iCmdId,std::string strName);
    void AddMenu(int iMenuId,std::string strName);
    void AddSubCommand(int iParentId,int iCmdId,std::string strName);

    void RefreshCommandList(tagCmdInf *pMenu = nullptr);

    tagCmdInf *m_pCurrentMenu;

    AppListInterface *m_pList;

    CButton *m_pReturnBtn;
    QLabel *m_pAppNameLab;
    CustomCombobox *m_pCommandList;

    QVBoxLayout *m_pMainLayout;
    QHBoxLayout *m_pTopLayout;
    QHBoxLayout *m_pBottomLayout;

    std::vector<tagCmdInf> m_CmdVec;
};

#endif // COMMANDVIEW_H
