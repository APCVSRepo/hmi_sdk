#include "AppTest.h"
#include "ui_capptest.h"
#include "QMessageBox"
#include "QHostInfo"

CAppTest::CAppTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CAppTest),
    m_pClient(NULL)
{
    ui->setupUi(this);
    ui->btn_Connect->setEnabled(true);
    ui->btn_Disconnect->setEnabled(false);
    ui->btn_Send->setEnabled(false);
    ui->lineEdit_Host->setText(QHostInfo::localHostName());
    ui->lineEdit_Port->setText("10000");
    ui->label_status->setText("Not connect.");

    ui->btn_ALERT->setEnabled(false);
    ui->btn_APPLINK->setEnabled(false);
    ui->btn_AUDIOPASSTHRU->setEnabled(false);
    ui->btn_CHOICESET->setEnabled(false);
    ui->btn_CHOICESETVR->setEnabled(false);
    ui->btn_COMMAND->setEnabled(false);
    ui->btn_SCROLLMSG->setEnabled(false);
    ui->btn_SHOW->setEnabled(false);
}

CAppTest::~CAppTest()
{
    delete ui;
}

void CAppTest::on_btn_Connect_clicked()
{
    if (!m_pClient)
    {
        m_pClient = new QTcpSocket(this);
        connect(m_pClient, SIGNAL(connected()), this, SLOT(socketConnectSuccess()));
        connect(m_pClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketConnectFailed(QAbstractSocket::SocketError)));
        connect(m_pClient, SIGNAL(disconnected()), this, SLOT(socketDisconnect()));
    }
    else
    {
        return;
    }

    if (m_Hostname.contains('.'))
    {
        m_pClient->connectToHost(QHostAddress(m_Hostname), m_Port);
    }
    else
    {
        m_pClient->connectToHost(m_Hostname, m_Port);
    }
}

void CAppTest::on_btn_Disconnect_clicked()
{
    if (m_pClient)
    {
        m_pClient->disconnectFromHost();
    }
}

void CAppTest::on_btn_Exit_clicked()
{
    this->close();
}

void CAppTest::on_lineEdit_Host_textChanged(const QString &arg1)
{
    m_Hostname = arg1;
}

void CAppTest::on_lineEdit_Port_textChanged(const QString &arg1)
{
    m_Port = arg1.toInt();
}

void CAppTest::on_btn_Send_clicked()
{
    int iSendSz = 0;
    QByteArray tmpStr;
    SockMsg_st tmpMsg;

    tmpStr =  ui->lineEdit_Send->text().toLatin1();
    tmpMsg.stMsgID = ui->lineEdit_MsgID->text().toInt();
    tmpMsg.stMsgSz = tmpStr.size() > BUFFERSIZE ? BUFFERSIZE : tmpStr.size();
    memcpy(tmpMsg.stByteArray, tmpStr.data(), tmpMsg.stMsgSz);

    iSendSz = m_pClient->write((char*)&tmpMsg, sizeof(SockMsg_st));
    ui->label_status->setText("Send bytes: " + QString::number(iSendSz));
}

void CAppTest::on_btn_Clear_clicked()
{
    ui->lineEdit_MsgID->clear();
    ui->lineEdit_Send->clear();
}

void CAppTest::socketConnectSuccess()
{
    ui->btn_Connect->setEnabled(false);
    ui->btn_Disconnect->setEnabled(true);
    ui->btn_Send->setEnabled(true);
    ui->lineEdit_Host->setEnabled(false);
    ui->lineEdit_Port->setEnabled(false);

    ui->btn_ALERT->setEnabled(true);
    ui->btn_APPLINK->setEnabled(true);
    ui->btn_AUDIOPASSTHRU->setEnabled(true);
    ui->btn_CHOICESET->setEnabled(true);
    ui->btn_CHOICESETVR->setEnabled(true);
    ui->btn_COMMAND->setEnabled(true);
    ui->btn_SCROLLMSG->setEnabled(true);
    ui->btn_SHOW->setEnabled(true);

//    QMessageBox::information(this, "Info", "Connect success!", QMessageBox::Ok);
    ui->label_status->setText("Connect success!");
}

void CAppTest::socketConnectFailed(QAbstractSocket::SocketError err)
{
    ui->btn_Connect->setEnabled(true);
    ui->btn_Disconnect->setEnabled(false);
    ui->btn_Send->setEnabled(false);
    ui->lineEdit_Host->setEnabled(true);
    ui->lineEdit_Port->setEnabled(true);
    ui->btn_ALERT->setEnabled(false);
    ui->btn_APPLINK->setEnabled(false);
    ui->btn_AUDIOPASSTHRU->setEnabled(false);
    ui->btn_CHOICESET->setEnabled(false);
    ui->btn_CHOICESETVR->setEnabled(false);
    ui->btn_COMMAND->setEnabled(false);
    ui->btn_SCROLLMSG->setEnabled(false);
    ui->btn_SHOW->setEnabled(false);
    if (m_pClient)
    {
        m_pClient->destroyed();
        m_pClient = NULL;
    }
//    QMessageBox::information(this, "Info", "Connect failed! \nError: " + QString::number(err), QMessageBox::Ok);
    ui->label_status->setText("Connect failed! Error: " + QString::number(err));
}

void CAppTest::socketDisconnect()
{
    if (m_pClient)
    {
        m_pClient->destroyed();
        m_pClient = NULL;
    }
    ui->btn_Connect->setEnabled(true);
    ui->btn_Disconnect->setEnabled(false);
    ui->btn_Send->setEnabled(false);
    ui->lineEdit_Host->setEnabled(true);
    ui->lineEdit_Port->setEnabled(true);
    ui->btn_ALERT->setEnabled(false);
    ui->btn_APPLINK->setEnabled(false);
    ui->btn_AUDIOPASSTHRU->setEnabled(false);
    ui->btn_CHOICESET->setEnabled(false);
    ui->btn_CHOICESETVR->setEnabled(false);
    ui->btn_COMMAND->setEnabled(false);
    ui->btn_SCROLLMSG->setEnabled(false);
    ui->btn_SHOW->setEnabled(false);
    ui->label_status->setText("Disconnect.");

}

void CAppTest::on_btn_APPLINK_clicked()
{
    SockMsg_st tmpMsg;
    tmpMsg.stMsgID = ui->lineEdit_APPLINK->text().toInt();
    m_pClient->write((char*)&tmpMsg, sizeof(SockMsg_st));
    ui->label_status->setText("Send command: APPLINK");
}

void CAppTest::on_btn_SHOW_clicked()
{
    SockMsg_st tmpMsg;
    tmpMsg.stMsgID = ui->lineEdit_SHOW->text().toInt();
    m_pClient->write((char*)&tmpMsg, sizeof(SockMsg_st));
    ui->label_status->setText("Send command: SHOW");
}

void CAppTest::on_btn_ALERT_clicked()
{
    SockMsg_st tmpMsg;
    tmpMsg.stMsgID = ui->lineEdit_ALERT->text().toInt();
    m_pClient->write((char*)&tmpMsg, sizeof(SockMsg_st));
    ui->label_status->setText("Send command: ALERT");
}

void CAppTest::on_btn_AUDIOPASSTHRU_clicked()
{
    SockMsg_st tmpMsg;
    tmpMsg.stMsgID = ui->lineEdit_AUDIOPASSTHRU->text().toInt();
    m_pClient->write((char*)&tmpMsg, sizeof(SockMsg_st));
    ui->label_status->setText("Send command: AUDIOPASSTHRU");
}

void CAppTest::on_btn_CHOICESET_clicked()
{
    SockMsg_st tmpMsg;
    tmpMsg.stMsgID = ui->lineEdit_CHOICESET->text().toInt();
    m_pClient->write((char*)&tmpMsg, sizeof(SockMsg_st));
    ui->label_status->setText("Send command: CHOICESET");
}

void CAppTest::on_btn_CHOICESETVR_clicked()
{
    SockMsg_st tmpMsg;
    tmpMsg.stMsgID = ui->lineEdit_CHOICESETVR->text().toInt();
    m_pClient->write((char*)&tmpMsg, sizeof(SockMsg_st));
    ui->label_status->setText("Send command: CHOICESETVR");
}

void CAppTest::on_btn_SCROLLMSG_clicked()
{
    SockMsg_st tmpMsg;
    tmpMsg.stMsgID = ui->lineEdit_SCROLLMSG->text().toInt();
    m_pClient->write((char*)&tmpMsg, sizeof(SockMsg_st));
    ui->label_status->setText("Send command: SCROLLMSG");
}

void CAppTest::on_btn_COMMAND_clicked()
{
    SockMsg_st tmpMsg;
    tmpMsg.stMsgID = ui->btn_COMMAND->text().toInt();
    m_pClient->write((char*)&tmpMsg, sizeof(SockMsg_st));
    ui->label_status->setText("Send command: COMMAND");
}
