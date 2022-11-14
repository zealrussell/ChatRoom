#ifndef RECVMSG_H
#define RECVMSG_H

#include <QThread>
#include <QTcpSocket>
#include "chatmessage.h"
#include <QDebug>

class RecvMsg : public QThread
{
    Q_OBJECT
public:
    explicit RecvMsg(QTcpSocket* tcp,QObject *parent = nullptr);

protected:
    void run() override;

private:
    QTcpSocket *socket;

signals:
    void signalOver(QTcpSocket *socket, ChatMessage myMessage);
};

#endif // RECVMSG_H
