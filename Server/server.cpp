#include "server.h"
#include "ui_server.h"


Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);


    server = new QTcpServer(this);
    int port = 8848;
    // 监听本地8848端口
    server->listen(QHostAddress::LocalHost, port);
    count = 0;

    // 当有一个新连接时，

    connect(server, &QTcpServer::newConnection, this, [=](){

        QTcpSocket *socket = server->nextPendingConnection();
        socketList.append(socket);
        count++;
        qDebug() << "开始执行";
        ui->allChat->append(socket->peerAddress().toString() + "::"
                            + QString::number(socket->peerPort())
                            + " 建立了新的连接。。。\n");
        // 子线程接受信息
        RecvMsg * worker= new RecvMsg(socket);
        worker->start();

        // 一次消息传输完毕后，广播消息
        connect(worker, &RecvMsg::signalOver, this, &Server::slotBroadcastMsg);

        // 断开连接
        connect(socket, &QTcpSocket::disconnected, this, [=](){
            worker->exit();
            ui->allChat->append(socket->peerAddress().toString() + "::"
                                + QString::number(socket->peerPort())
                                + "客户端断开了连接。。。。\n");

            socketList.removeOne(socket);
            socketMap.remove(socket);
            socket->deleteLater();
            count--;
            slotBroadcastUser();
        });
    });


}

// 广播消息
void Server::slotBroadcastMsg(QTcpSocket *socket, ChatMessage msg)
{
    qDebug() <<"socket" << &socket;
    if (msg.type == 1) {
        QString chat = time.currentTime().toString() + "\n"
                        + msg.username + ": " + msg.message + "\n";
        ui->allChat->append(chat);
        QByteArray block;
        QDataStream in(&block, QIODevice::WriteOnly);
        in.setVersion(QDataStream::Qt_5_0);
        in << msg.type << msg.username << msg.message;
        qDebug() << "广播消息" << msg.message;
        foreach (QTcpSocket *it, socketList) {
            it->write(block, block.size());
        }
    }  else if (msg.type == 2) {
        socketMap.insert(socket, msg.username);
        slotBroadcastUser();
    }
}

// 广播用户列表
void Server::slotBroadcastUser()
{

    numMessage.type = 2;
    QStringList usernameList;
    foreach (QString name, socketMap.values()) {
        usernameList.append(name);
    }
    qDebug() << usernameList;
    numMessage.username = usernameList.join(",");
    numMessage.message = QString::number(count);
    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_5_0);
    in << numMessage.type << numMessage.username << numMessage.message;
    qDebug() << "人数变化:" << count << numMessage.username;
    foreach (QTcpSocket *it, socketList) {
        it->write(block, block.size());
    }
}

Server::~Server()
{
    socketList.clear();
    server->close();
    delete ui;
}



