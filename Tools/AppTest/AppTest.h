#ifndef APPTEST_H
#define APPTEST_H

#include <QWidget>
#include <QtNetwork>

#define MSG_ID_SHOW   (0x1001)
#define MSG_ID_ALERT   (0x1002)
#define MSG_ID_AUDIOPASSTHRU   (0x1003)
#define MSG_ID_CHOICESET   (0x1004)
#define MSG_ID_CHOICESETVR   (0x1005)
#define MSG_ID_SCROLLMSG   (0x1006)
#define MSG_ID_COMMAND (0x1007)

#define BUFFERSIZE (8)
struct SockMsg_st
{
    int     stMsgID;
    int     stMsgSz;
    unsigned char  stByteArray[BUFFERSIZE];

    SockMsg_st()
    {
        memset(this, 0, sizeof(SockMsg_st));
    }
};

namespace Ui {
class CAppTest;
}

class CAppTest : public QWidget
{
    Q_OBJECT

public:
    explicit CAppTest(QWidget *parent = 0);
    ~CAppTest();

signals:


private slots:
    void on_btn_Connect_clicked();
    void on_btn_Disconnect_clicked();
    void on_btn_Exit_clicked();
    void on_lineEdit_Host_textChanged(const QString &arg1);
    void on_lineEdit_Port_textChanged(const QString &arg1);
    void on_btn_Send_clicked();
    void on_btn_Clear_clicked();
    void socketConnectSuccess();
    void socketConnectFailed(QAbstractSocket::SocketError err);
    void socketDisconnect();

    void on_btn_APPLINK_clicked();

    void on_btn_SHOW_clicked();

    void on_btn_ALERT_clicked();

    void on_btn_AUDIOPASSTHRU_clicked();

    void on_btn_CHOICESET_clicked();

    void on_btn_CHOICESETVR_clicked();

    void on_btn_SCROLLMSG_clicked();

    void on_btn_COMMAND_clicked();

private:
    Ui::CAppTest *ui;

    QTcpSocket *m_pClient;
    QString m_Hostname;
    int m_Port;
};

#endif // APPTEST_H
