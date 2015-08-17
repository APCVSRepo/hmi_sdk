#ifndef NOTIFY_H
#define NOTIFY_H

#include <QWidget>
#include "QLabel"
#include "../Common/PopBase.h"
#include "QVBoxLayout"

class Notify : public CPopBase
{
    Q_OBJECT
public:
    explicit Notify(QWidget *parent = 0);

    void execShow(AppDataInterface* pAppInterface);

signals:
    
public slots:
    
private:
    void setText(QString text);

private:
    QLabel m_labelText;
    QVBoxLayout *mLayout;
    void InitLayout();
    void waitSec(int s);
};

#endif // NOTIFY_H
