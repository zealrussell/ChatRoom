#include "fileserver.h"
#include "ui_fileserver.h"

FileServer::FileServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileServer)
{
    ui->setupUi(this);
    tcpPort = 6666;
    tcpServer = new QTcpServer(this);

    // 当对方同意时，发送文件
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotSendFile()));
    initServer();

}

FileServer::~FileServer()
{
    delete ui;
}

void FileServer::initServer(quint16 udpPort)
{
    payloadSize = 64 * 1024;
    TotalBytes = 0;
    bytestoWrite = 0;
    bytesWritten = 0;
    targetPort = udpPort;
    ui->label->setText(tr("请选择要传送的文件"));
    ui->progressBar->reset();
    ui->fileOpenBtn->setEnabled(true);
    ui->fileSendBtn->setEnabled(false);
    tcpServer->close();
}


void FileServer::refused()
{
    tcpServer->close();
    ui->label->setText(tr("对方拒绝接收!!!"));
}

void FileServer::setTargePort(quint16 port)
{
    targetPort = port;
}

quint16 FileServer::getTcpPort()
{
    return tcpPort;
}

// 发送文件
void FileServer::slotSendFile()
{
    // qDebug () << "TCP的链接已建立" ;
    ui->fileSendBtn->setEnabled(false);
    clientConnection = tcpServer->nextPendingConnection();

//    connect(clientConnection,SIGNAL(bytesWritten(qint64)),
//            this,SLOT(slotUpdateClientProgress(qint64)));

    ui->label->setText(tr("开始传送文件:\n %1!").arg(theFileName));

//    localFile = new QFile(fileName);
//    if(! localFile->open(QFile::ReadOnly))
//    {
//        QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1:\n%2").arg(fileName, localFile->errorString()) );
//        return;
//    }

//    TotalBytes = localFile->size();

//    // time.start();
//    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
//    sendOut.setVersion(QDataStream::Qt_4_0);
//    time.start();

//    QString currentFile = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
//    sendOut << qint64(0) <<qint64(0) <<currentFile;


//    TotalBytes += outBlock.size();
//    sendOut.device()->seek(0);

//    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));
//    bytestoWrite = TotalBytes - clientConnection->write(outBlock);
//    outBlock.resize(0);

    // 多线程发送文件
    FileWorker *sendWorker = new FileWorker(clientConnection, fileName);
    QThread *thread = new QThread;
    sendWorker->moveToThread(thread);
    qDebug() << thread->currentThreadId();

    connect(this, &FileServer::signalBeginSend, sendWorker, &FileWorker::slotSendFile);
    emit signalBeginSend();

    // 打开文件失败
    connect(sendWorker, &FileWorker::signalOpenFileFail, this, [=](){
        QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1:\n%2").arg(fileName, localFile->errorString()) );
        return;
    });

    // 更新进度条
    connect(sendWorker, &FileWorker::signalFilePercent, ui->progressBar, &QProgressBar::setValue);
    // 传输文件完成
    connect(sendWorker, &FileWorker::signalSendOver, this, [=]{
        ui->label->setText(tr("文件传输完成"));
        thread->quit();
        thread->wait();
        sendWorker->deleteLater();
        thread->deleteLater();
    });

    thread->start();

}

// 传输进度条
void FileServer::slotUpdateClientProgress(qint64 numBytes)
{
    qApp->processEvents();
    bytesWritten +=(int) numBytes;
    if(bytestoWrite > 0)
    {
        outBlock = localFile->read(qMin(bytestoWrite,payloadSize));
        bytestoWrite -=(int)clientConnection->write(outBlock);
        outBlock.resize(0);

    }
    else
    {
        localFile->close();
    }
    ui->progressBar->setMaximum(TotalBytes);
    ui->progressBar->setValue(bytesWritten);

    float useTime = time.elapsed();

    double speed = bytesWritten / useTime;
    ui->label->setText(tr("已发送 %1MB( %2MB/s)"
                                      "\n共%3MB 已用时:%4秒\n估计剩余时间:%5秒")
                                   .arg(bytesWritten / (1024*1024))
                                   .arg(speed * 1000 /(1024*1024),0,'f',2)
                                   .arg(TotalBytes /(1024*1024))
                                   .arg(useTime/1000,0,'f',0)
                                   .arg(TotalBytes/speed/1000 - useTime/1000,0,'f',0));
    if (bytesWritten == TotalBytes) {
        localFile->close();
        tcpServer->close();
        ui->label->setText(tr("传送文件: %1成功").arg(theFileName));
    }

}



// 打开文件
void FileServer::on_fileOpenBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        theFileName = fileName.right(fileName.size()-fileName.lastIndexOf('/')-1);
        // qDebug() << "文件名" <<  theFileName ;

        ui->label->setText(tr("要发送的文件为:%1").arg(theFileName));
        ui->fileSendBtn->setEnabled(true);
        ui->fileOpenBtn->setEnabled(false);
     }

}

// 准备发送文件
void FileServer::on_fileSendBtn_clicked()
{
    if(! tcpServer->listen(QHostAddress::Any, 0))
    {
        qDebug() << tcpServer->errorString()<<"文件传输连接失败";
        close();
        return ;
    }
    tcpPort = tcpServer->serverPort();
    qDebug() <<"向 " << targetPort << " 传输，文件传输服务器监听端口：" << tcpPort;

    ui->fileSendBtn->setEnabled(false);
    ui->label->setText(tr("等待对方的接受......"));
    emit signalSendFileName(theFileName, targetPort, tcpPort);
}

// 退出fileserver
void FileServer::on_fileCloseBtn_clicked()
{
    if(tcpServer->isListening())
    {
        qDebug() << "点击了关闭按钮";
        tcpServer->close();
//        if(localFile->isOpen())
//        {
//            localFile->close();
//        }
        clientConnection->abort();

    }
    close();
    ui->~FileServer();
}

void FileServer::closeEvent(QCloseEvent *)
{
    on_fileCloseBtn_clicked();
}
