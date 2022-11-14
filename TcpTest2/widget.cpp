#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    server->listen(QHostAddress::LocalHost, 8888);
    connect(server, &QTcpServer::newConnection, this, &Widget::newConnect);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::newConnect()
{
    QTcpSocket *socket = server->nextPendingConnection();
    qDebug() <<"新连接：" << socket;
    socketList.append(socket);
    connect(socket, &QTcpSocket::readyRead, this, [=](){
        QByteArray data;
        data = socket->readAll();
        ui->textBrowser->append(data);
        qDebug() << "接收" << data;
        broadcast(&data);

    });

}

void Widget::revMessage(QTcpSocket *socket)
{

    qDebug() << socket;
}

void Widget::broadcast(QByteArray *data)
{
    qDebug() << "开始广播";
    if (socketList.isEmpty() ) qDebug() << "为空";
    foreach(QTcpSocket *socket, socketList) {
        qDebug() <<"向" <<socket << "发送了" << data;
        socket->write(data->data());
    }
}

