#include "recvmsg.h"

RecvMsg::RecvMsg(QTcpSocket* tcp,QObject *parent) : QThread(parent)
{
    socket = tcp;

}

void RecvMsg::run()
{

    connect(socket, &QTcpSocket::readyRead, this, [=](){
        ChatMessage myMessage;
        // 接收客户端发来的消息
        qint64 size = socket->bytesAvailable();
        int mysize = static_cast<int>(size);
        QByteArray block = QByteArray(mysize,0);
        socket->read(block.data(), size);
        QDataStream out(&block, QIODevice::ReadOnly);
        // qDebug() << "接受： " <<array;
        out >> myMessage.type >> myMessage.username >> myMessage.message;
        // qDebug() << "我的" << myMessage.username;
        emit signalOver(socket, myMessage);

    });

    exec();
}
