#ifndef POPBASE_H
#define POPBASE_H

#include <QWidget>
#include "QTimer"
#include "QLabel"
#include "Background.h"
#include "AppData/AppDataInterface.h"

class CPopBase : public QWidget
{
    Q_OBJECT
public:
    explicit CPopBase(QWidget *parent = 0);

public:
    virtual void execShow(AppDataInterface* pAppInterface);

signals:
    void onButtonClicked(int btnID);
    void onListClicked(int listID);
    void onSpaceCliced();
public slots:
    
protected:
    QTimer *m_timer;
    QLabel m_labelFrame;
    QLabel m_labelBackground;
    Json::Value m_jsonData;
private:
    Background m_labelBackspace;
};

#endif // POPBASE_H
