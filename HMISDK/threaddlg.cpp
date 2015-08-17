#include "include/threaddlg.h"
#include "include/connect/connect.h"
#include "include/SDLConnector.h"
#include <QHBoxLayout>
#include <iostream>
#include "include/json/json.h"

#ifdef WINCE
#include <stdlib.h>
#endif
ThreadDlg::ThreadDlg(QWidget *parent):
    QDialog(parent)
{
    setWindowTitle(tr("Thread"));

    startBtn = new QPushButton(tr("Start"));
    stopBtn = new QPushButton(tr("Stop"));
    quitBtn = new QPushButton(tr("Quit"));

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(startBtn);
    mainLayout->addWidget(stopBtn);
    mainLayout->addWidget(quitBtn);

    connect(startBtn,SIGNAL(clicked()),this,SLOT(slotStart()));
    connect(stopBtn,SIGNAL(clicked()),this,SLOT(slotStop()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(close()));
}

ThreadDlg::~ThreadDlg()
{

}

void ThreadDlg::slotStart()
{
    SDLConnector::getSDLConnectore()->ConnectToSDL(this);
}

void ThreadDlg::slotStop()
{
}

void ThreadDlg::recvFromServer(Json::Value jsonObj)
{
    std::cout << "+++++recvFromServer+++++\n";
    std::cout << jsonObj.toStyledString() << std::endl;
    std::cout << "---------\n";
    stopBtn->setText("Yes");

    if (jsonObj.isMember("method") && jsonObj["method"].asString() == "BasicCommunication.OnAppRegistered"){
        int appID = jsonObj["params"]["application"]["appID"].asInt();
        sleep(3);
        SDLConnector::getSDLConnectore()->OnAppActivated(appID);
        stopBtn->setText("Activated");
    }
}

