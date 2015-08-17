#ifndef THREADDLG_H
#define THREADDLG_H

#include <QDialog>
#include <QPushButton>

#include "HMISDK/include/SDLConnector.h"
#include "HMISDK/include/protocolHandler/MessageCallback.h"

class ThreadDlg : public QDialog, public MessageCallback
{
    Q_OBJECT

public:
    ThreadDlg(QWidget *parent = 0);
    ~ThreadDlg();
private:
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QPushButton *quitBtn;

public slots:
    void slotStart();
    void slotStop();

public:
    void recvFromServer(Json::Value);
};

#endif // THREADDLG_H
