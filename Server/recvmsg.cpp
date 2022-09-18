#include "recvmsg.h"

RecvMsg::RecvMsg(QTcpSocket* tcp,QObject *parent) : QThread(parent)
{
    socket = tcp;
}

void RecvMsg::run()
{

    QTime *time = new QTime();
    connect(socket, &QTcpSocket::readyRead, this, [=](){
        // 接收客户端发来的消息
        QByteArray data = socket->readAll() + "\n";
        // 拼接上时间
        QString msg = "type:1/n";
        msg += time->currentTime().toString("hh:mm::ss") + "\n";
        msg += data;
        emit over(msg.toUtf8());
    });
    exec();
}
