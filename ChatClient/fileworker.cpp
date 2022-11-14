#include "fileworker.h"
#include <QDebug>

FileWorker::FileWorker(QTcpSocket *tcp, QString filename, QObject * parent):
    QObject(parent),
    socket(tcp),
    fileName(filename)
{
    qDebug() << fileName;
    if(socket != nullptr)
        qDebug() <<"子线程：客户端端口" << socket->peerPort();
//    server = new QTcpServer();
//    server->listen(QHostAddress::LocalHost, 0);
//    localPort = server->serverPort();
//    connect(server, &QTcpServer::newConnection, this, [=](){
//       socket = server->nextPendingConnection();
//    });
}

FileWorker::~FileWorker()
{

}

void FileWorker::slotSendFile()
{
    qDebug() << "子线程开始传输文件";
    QFile file(fileName);
    QFileInfo info(fileName);
    const int fileSize = info.size();

    if(! file.open(QFile::ReadOnly))
    {
        emit signalOpenFileFail();
        return;
    }

    while (!file.atEnd())
    {
        static int num = 0;
        if(num == 0)
        {
            socket->write((char *)&fileSize,4);
        }
        QByteArray line = file.readLine();

        // 文件大小
        num += line.size();
        int percent = (num * 100 / fileSize);
        emit signalFilePercent(percent);

        socket->write(line);
    }
    if(file.isOpen())
    {
        file.close();
    }
    emit signalSendOver();
}

