#ifndef CUSTOMCOMBOBOXITEM_H
#define CUSTOMCOMBOBOXITEM_H

#include <QListWidgetItem>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>

class CustomComboboxItem : public QLabel
{
    Q_OBJECT
public:
    explicit CustomComboboxItem(int w,int h,QWidget *parent = 0);
    ~CustomComboboxItem();

    void setText(QString text);

    void setIsMenu(bool isMenu);

    void SetBkPic(QString strNormalPicPath,QString strHoverPicPath);
    enum BkState{NORMAL_STATE = 0,HOVER_STATE};
    void ChangeBkPic(int iFlag);

    QString GetItemText();
private:
    QLabel *m_pIconLab;
    QLabel *m_pMenuArrowLab;
    QLabel *m_pTextLab;
    QHBoxLayout *m_pMainLayout;

    QString m_strNormalBk;
    QString m_strHoverBk;

    int m_iBkState;
};

#endif // CUSTOMCOMBOBOXITEM_H
