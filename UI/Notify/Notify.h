#ifndef NOTIFY_H
#define NOTIFY_H

#include <QWidget>
#include "QLabel"
#include "QVBoxLayout"
#include "Common/PopBase.h"
class CPopBase;
class Notify : public CPopBase
{
    Q_OBJECT
public:
    explicit Notify(QWidget *parent = 0);

    void execShow();
    void testShow();

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
