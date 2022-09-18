#include "server.h"
#include "ui_server.h"
#include "recvmsg.h"

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
    //socketList = new QList<QTcpSocket *>();

    // 当有一个新连接时，
    connect(server, &Server::userNum, this, &Server::broadcastNum);
    connect(server, &QTcpServer::newConnection, this, [=](){
        QTcpSocket *socket = server->nextPendingConnection();
        socketList.append(socket);
        count++;
        emit userNum(count);
        ui->allChat->append(socket->peerAddress().toString() + "::"
                            + QString::number(socket->peerPort())
                            + "建立了新的连接。。。\n");
        // 子线程接受信息
        RecvMsg * worker= new RecvMsg(socket);
        worker->start();

        // 一次消息传输完毕后，广播消息
        connect(worker, &RecvMsg::over, this, &Server::broadcastMsg);

        // 断开连接
        connect(socket, &QTcpSocket::disconnected, this, [=](){
            worker->exit();
            ui->allChat->append(socket->peerAddress().toString() + "::"
                                + QString::number(socket->peerPort())
                                + "客户端断开了连接。。。。\n");
            socketList.removeOne(socket);
            count--;
            socket->deleteLater();
            emit userNum(count);
        });
    });

}

void Server::broadcastMsg(QString msg)
{
    ui->allChat->append(msg);
    foreach (QTcpSocket *it, socketList) {
        it->write(msg.toUtf8());
    }
}

void Server::broadcastNum(int number)
{
    QString msg = "type:2/n";
    msg += QString::number(number);
    foreach (QTcpSocket *it, socketList) {
        it->write(msg.toUtf8());
    }
}

Server::~Server()
{
    socketList.clear();
    server->close();
    delete ui;
}



