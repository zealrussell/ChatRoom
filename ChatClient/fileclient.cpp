#include "fileclient.h"
#include "ui_fileclient.h"
#include<QHostInfo>
#include<QMessageBox>
#include<QDateTime>
#include<QProcess>
#include<QDataStream>
#include<QScrollBar>
#include<QFont>
#include<QNetworkInterface>
#include<QStringList>
#include<QDebug>

FileClient::FileClient(QWidget *parent, quint16 port) :
    QDialog(parent),
    tcpPort(port),
    ui(new Ui::FileClient)
{
    ui->setupUi(this);

    TotalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    tcpClient = new QTcpSocket(this);
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(slotReceveFile()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotDisplayError(QAbstractSocket::SocketError)));

}

FileClient::~FileClient()
{
    delete ui;
}

void FileClient::setFileName(QString fileName)
{
    localFile = new QFile(fileName);
}

void FileClient::serServerUdpPort(quint16 port)
{
    udpPort = port;
}

void FileClient::setServerTcpPort(quint16 port)
{
    tcpPort = port;
}

void FileClient::setHostAddress(QHostAddress address)
{
    hostAddress = address;
    newConnect();
}

void FileClient::newConnect()
{
    blockSize = 0;
    tcpClient->abort();
    tcpClient->connectToHost(hostAddress,tcpPort);
    qDebug() <<"文件客户端:tcp端口" << tcpClient->localPort();
    if (tcpClient->state() == QAbstractSocket::ConnectedState)
        time.start();
}

// 接收文件
void FileClient::slotReceveFile()
{
    qDebug() << "开始接收文件";
    if(! localFile->open(QFile::WriteOnly))
    {
        QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1:\n%2.")
                             .arg(fileName, localFile->errorString()));
        return ;
    }
    static int count = 0;
    static int total = 0;
    if (count == 0) {
        tcpClient->read((char *)&total,4);
    }
    QByteArray all = tcpClient->readAll();
    count += all.size();
    localFile->write(all);
    if (count == total) {
        ui->label->setText(tr("接收文件: %1完毕").arg(fileName));
        ui->progressBar->setValue(100);
        tcpClient->close();
        tcpClient->deleteLater();
        localFile->close();
        localFile->deleteLater();

    }
//    QDataStream in(tcpClient);
//    in.setVersion(QDataStream::Qt_4_0);
//    float useTime = time.elapsed();

//    if(bytesReceived <= sizeof(qint64)*2)
//    {
//        if((tcpClient->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize ==0))
//        {
//            in >> TotalBytes >> fileNameSize;
//            bytesReceived += sizeof(qint64)*2;
//        }
//        if((tcpClient->bytesAvailable() >= fileNameSize) && (fileNameSize!=0))
//        {
//            in >> fileName;
//            bytesReceived += fileNameSize;
//            if(! localFile->open(QFile::WriteOnly))
//            {
//                QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1:\n%2.")
//                                     .arg(fileName, localFile->errorString()));
//                return ;
//            }
//            else return ;

//        }


//    }
//    if(bytesReceived < TotalBytes)
//    {
//        bytesReceived += tcpClient->bytesAvailable();
//        inBlock = tcpClient->readAll();
//        localFile->write(inBlock);
//        inBlock.resize(0);

//    }

//    ui->progressBar->setMaximum(TotalBytes);
//    ui->progressBar->setValue(bytesReceived);

//    double speed = bytesReceived / useTime;
//    qDebug() << "接收速度：" << speed;
//    ui->label->setText(tr("已接收 %1MB( %2MB/s)"
//                                         "\n共%3MB 已用时:%4秒\n估计剩余时间:%5秒")
//                                      .arg(bytesReceived / (1024*1024))
//                                      .arg(speed *1000/(1024*1024),0,'f',2)
//                                      .arg(TotalBytes / (1024*1024))
//                                      .arg(useTime/1000,0,'f',0)
//                                      .arg(TotalBytes/speed/1000 - useTime/1000,0,'f',0 ));
//    if(bytesReceived >= TotalBytes)
//    {
//       localFile ->close();
//       tcpClient->close();
//       ui->label->setText(tr("接收文件: %1完毕").arg(fileName));
//    }



}

void FileClient::slotDisplayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "文件客户端出错";
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        default:
            qDebug() << tcpClient->errorString();
    }
}

// 取消按钮
void FileClient::on_fileCancelBtn_clicked()
{
    tcpClient->abort();
    if (localFile->isOpen()) {
        localFile->close();
    }
    close();
    ui->~FileClient();
}

// 关闭按钮
void FileClient::on_fileCloseBtn_clicked()
{
    on_fileCancelBtn_clicked();
}

void FileClient::closeEvent(QCloseEvent *)
{
    on_fileCloseBtn_clicked();
}
