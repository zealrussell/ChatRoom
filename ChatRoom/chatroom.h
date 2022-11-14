#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QMessageBox>
#include <QDateTime>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <QStringList>
#include <QDebug>
#include <QStringListModel>
#include "MyMessage.h"

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

    void on_connectBtn_clicked();

    void on_clearBtn_clicked();  //清除按钮

    void on_sendBtn_clicked();   //发送按钮

    void connectToServer();

    void slotRevMessage();           //接受信息

    void registerUsername();

private:
    Ui::ChatRoom *ui;
    QTime *time;
    QTcpSocket *socket;
    bool connectState;
    int count;
    QString username;
    QStringList usernameList;

};
#endif // CHATROOM_H
