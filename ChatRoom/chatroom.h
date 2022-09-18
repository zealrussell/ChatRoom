#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatRoom; }
QT_END_NAMESPACE

class ChatRoom : public QWidget
{
    Q_OBJECT

public:
    ChatRoom(QWidget *parent = nullptr);
    ~ChatRoom();

private slots:
    void connectToServer();

    void on_clearBtn_clicked();  //清除按钮

    void on_sendBtn_clicked();   //发送按钮

    void readMessage();           //接受信息

    void on_connectBtn_clicked();

private:
    Ui::ChatRoom *ui;
    QTcpSocket *socket;
    bool connectState;
    QString username;
};
#endif // CHATROOM_H
