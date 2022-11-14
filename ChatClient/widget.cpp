#include "widget.h"
#include "ui_widget.h"
#include <QDateTime>
#include <QStringList>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QProcess>
#include <QScrollBar>
#include <QFileDialog>
#include <QEvent>
#include <QKeyEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      socket(nullptr),
      server(nullptr),
      ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->messageTextEdit->setFocusPolicy(Qt::StrongFocus);
    ui->messageTextBrowser->setFocusPolicy(Qt::NoFocus);
    ui->messageTextEdit->setFocus();
    ui->messageTextEdit->installEventFilter(this);

    server = new FileServer(this);
    socket = new QUdpSocket(this);
    socket->bind(0, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);

    serverPort = 8888;
    localAddress = getIp();
    localPort = socket->localPort();
    localName = getUsername();

    // ortToNameMap.insert(localPort, localAddress);
    ui->portLabel->setText(tr("端口号:%1").arg(localPort));

    // 接收消息
    connect(socket, &QUdpSocket::readyRead, this, &Widget::slotReceveMessage);
    // 发送上线消息
    sendMessage(NewParticipant);

    // 接收文件名称
    connect(server, SIGNAL(signalSendFileName(QString,quint16,quint16)), this, SLOT(slotGetFile(QString,quint16,quint16)));


}

Widget::~Widget()
{
    portToNameMap.clear();
    close();
    delete ui;
}

// 发送消息
void Widget::sendMessage(MessageType type, quint16 targetPort, quint16 tcpPort)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);


    // 写入用户名
    out << type << localName;
    out << localAddress << localPort;

    switch(type) {
        case ChatMessage:
            if(ui->messageTextEdit->toPlainText() == "") {
                QMessageBox::warning(0,tr("警告"),
                                     tr("发送内容不能为空"),QMessageBox::Ok);
                return ;
            }
            // 写入 地址、消息内容
            out << getMessage();
            // qDebug() << "发送消息: ";
            // 发送后移动到最下方
            ui->messageTextBrowser->verticalScrollBar()
                    ->setValue(ui->messageTextBrowser
                               ->verticalScrollBar()->maximum());
            socket->writeDatagram(data, data.length(), QHostAddress::LocalHost, serverPort);
            break;

        case NewParticipant:
            socket->writeDatagram(data, data.length(), QHostAddress::LocalHost, serverPort);
            break;
        case LeftParticipant:
            socket->writeDatagram(data, data.length(), QHostAddress::LocalHost, serverPort);
            break;
        case Refuse:
            out << targetPort;
            socket->writeDatagram(data, data.length(), QHostAddress::LocalHost, targetPort);
            break;
        case FileName:{
            // 发送服务器tcp端口
            out << targetPort;
            out << tcpPort << filename;
            socket->writeDatagram(data, data.length(), QHostAddress::LocalHost, targetPort);
            break;
        }
        case Xchat:
            qDebug() << "发送私聊请求：" << tcpPort;
            out << targetPort << tcpPort;
            socket->writeDatagram(data, data.length(), QHostAddress::LocalHost, targetPort);
            break;
    }
}


// 接收消息
void Widget::slotReceveMessage(){
    while(socket ->hasPendingDatagrams())
    {

        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size());

        // 反序列化
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        quint16 peerPort;
        QString userName, peerAddress, message;
        in >> messageType;
        in >> userName >> peerAddress >> peerPort;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        qDebug() << "群聊：接收消息" << messageType;
        switch (messageType) {
            case ChatMessage:
                in >> message;
                ui->messageTextBrowser->setTextColor(Qt::blue);
                ui->messageTextBrowser->setCurrentFont(QFont("Times New Roman",12));
                ui->messageTextBrowser->append("[ " + userName + " ]" + time);
                ui->messageTextBrowser->append(message);
                qDebug() << "接收文字消息";
                break;
            case NewParticipant:{
                QMap<quint16, QString> mapp;
                in >> mapp;
                newParticipant(mapp);
                break;
            }
            case LeftParticipant:{
                QMap<quint16, QString> mapp;
                in >> mapp;
                leftParticipant(mapp, time);
                break;
            }
            case FileName:{
                quint16 targePort, tcpPort;
                QString fileName;
                in >> targePort >> tcpPort >> fileName;
                qDebug() << "主线程：文件服务器端口" << tcpPort;
                receveFile(userName, peerPort, tcpPort, fileName);
                break;
            }
            case Refuse:{
                quint16 targetPort, tcpPort;
                in >>  targetPort >> tcpPort;
                if (targetPort != localPort) break;
                if(tcpPort == server->getTcpPort()) {
                    server->close();
                }
                break;
            }
            case Xchat:
                quint16 targetPort, udpPort;
                in >> targetPort >> udpPort;
                qDebug() << "对方私聊端口" << udpPort;
                if (targetPort != localPort) break;
                showXchat(localName, userName, udpPort);
                break;

        }

    }




}


// 用户登录
void Widget::newParticipant(const QMap<quint16, QString> &map)
{
    auto it = map.constBegin();
    while (it != map.constEnd()) {
        // 有就跳过
        if(portToNameMap.find(it.key()) != portToNameMap.end()) {
            it++;
            continue;
        }
        quint16 peerPort = it.key();
        QString userName = it.value();
        // bool isEmpty = ui ->userTableWidget->findItems(userName,Qt::MatchExactly).isEmpty();
        qDebug() <<"用户：" << userName << " :" << peerPort << "登录";

        portToNameMap.insert(peerPort, localAddress);

        QTableWidgetItem *user = new QTableWidgetItem(userName);
        QTableWidgetItem *ip = new QTableWidgetItem(localAddress);
        QTableWidgetItem *port = new QTableWidgetItem(QString::number(peerPort));

        ui->userTableWidget->insertRow(0);
        ui->userTableWidget->setItem(0,0,user);
        ui->userTableWidget->setItem(0,1,ip);
        ui->userTableWidget->setItem(0,2,port);

        ui->messageTextBrowser->setTextColor(Qt::gray);
        ui->messageTextBrowser->setCurrentFont(QFont("Times New Roman",10));

        ui->messageTextBrowser->append(tr("%1 在线!").arg(userName));
        ui->userNumberLabel->setText(tr("在线人数:%1")
                                  .arg(ui->userTableWidget->rowCount()));
        it++;
    }

}

// 用户退出
void Widget::leftParticipant(const QMap<quint16, QString> &map, QString time)
{
    auto it = portToNameMap.constBegin();
    while (it != portToNameMap.constEnd()) {
        // 如果该用户还在就继续
        if(map.find(it.key()) != map.end()) {
            it++;
            continue;
        }
        quint16 peerPort = it.key();
        QString userName = it.value();
        portToNameMap.remove(peerPort);
        qDebug() <<"用户：" << userName << " :" << peerPort << "退出";
        // 找到对应的行号
        int rowNum = ui->userTableWidget->findItems(QString::number(peerPort),Qt::MatchExactly).at(0)->row();

        ui->userTableWidget->removeRow(rowNum);
        ui->messageTextBrowser->setTextColor(Qt::gray);
        ui->messageTextBrowser->setCurrentFont(QFont("Times New Roman",10));

        ui->messageTextBrowser->append(tr("%1:%2于%3离开!").arg(userName).arg(peerPort).arg(time));
        ui->userNumberLabel->setText(tr("在线人数:%1")
                                  .arg(ui->userTableWidget->rowCount()));
        it++;
    }

}

// 有文件需要发送时，发送文件
void Widget::slotGetFile(QString name,quint16 targetPort, quint16 tcpPort){
    filename = name;
    qDebug() << "准备发送文件：" << filename;
    sendMessage(FileName, targetPort, tcpPort);
}

// 接收文件
void Widget::receveFile(QString serverName, quint16 serverUdpPort,
                            quint16 serverTcpPort, QString fileName)
{

    int btn = QMessageBox::information(this,tr("接收文件"),
                                       tr("来自 %1 (%2)的文件:%3","是否接受")
                                       .arg(serverName)
                                       .arg(serverTcpPort).arg(fileName),
                                       QMessageBox::Yes,QMessageBox::No);
    // 接收文件
    if(btn == QMessageBox::Yes) {
        QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),fileName);
        if(!name.isEmpty()) {
            FileClient *client = new FileClient(this, serverTcpPort);
            client->setFileName(name);
            client->newConnect();
            client->show();

        } else {
           FileClient  *client = new FileClient(this, serverTcpPort);
           client->localFile =new QFile(fileName);
        }
    // 拒绝接收
    } else if (btn == QMessageBox::No) {
        sendMessage(Refuse, serverUdpPort);
    }



}



// 双击进入私聊
void Widget::on_userTableWidget_doubleClicked(const QModelIndex &index)
{
    QString peerName = ui->userTableWidget->item(index.row(),0)->text();
    quint16 peerPort = ui->userTableWidget->item(index.row(),2)->text().toUInt();
    if(peerName == localName && peerPort == localPort)
    {
        QMessageBox::warning(0,tr("警告"),tr("不可以和自己聊天！！！"),QMessageBox::Ok);
        return ;
    } else {
        qDebug() <<"进入私聊";
        ChatThread workerThread;

        workerThread.setLocalName(localName);
        workerThread.setPeerName(peerName);
        workerThread.start();

        sendMessage(Xchat, peerPort, workerThread.getLocalPort());
        workerThread.stop();

//        Chat  *privateChat = new Chat(localName, peerName);
//        privateChat->show();

        // 向对方发送私聊请求


    }
}

// 被调用
void Widget::showXchat(QString myName, QString peerName, quint16 udpPort)
{
    ChatThread workerThread1;
    workerThread1.setIsBeCalled(true);
    workerThread1.setLocalName(myName);
    workerThread1.setPeerName(peerName);
    workerThread1.setPeerPort(udpPort);

    workerThread1.start();
//    Chat *chat = new Chat(myName, peerName, udpPort);
//    chat->connectToChat();
//    chat->show();

    workerThread1.stop();
}


// 发送文件
void Widget::on_sendFileButton_clicked()
{
    if(ui->userTableWidget->selectedItems().isEmpty())
    {
            QMessageBox::warning(0,tr("选择用户"),
                                 tr("请从用户列表选择要传送的用户!"),QMessageBox::Ok);
            return ;
    } else {
        int rowNum = ui->userTableWidget->selectedItems().at(0)->row();
        int targetPort = ui->userTableWidget->item(rowNum,2)->text().toUInt();
        if ( targetPort == localPort) {
            QMessageBox::warning(0,tr("警告"),tr("不可以向自己传输文件！！！"),QMessageBox::Ok);
            return;
        }
        server->show();
        server->initServer(targetPort);

    }

}

// ----------------------------------

// 获取本机ip
QString Widget::getIp()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return 0;
}

// 获取一个空闲的端口号
quint16 Widget::getFreePort()
{
    if (socket->isOpen())
        return socket->peerPort();
    return 0;
}

QString Widget::getUsername()
{
    if (ui->userNameTextEdit->toPlainText() != "") {
        localName = ui->userNameTextEdit->toPlainText();
        return localName;
    }
    else return "user" + QString::number(localPort);

}

// 获取聊天内容
QString Widget::getMessage()
{
    QString  msg = ui->messageTextEdit->toHtml();
    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
    return msg;
}


// 发送按钮
void Widget::on_sendButton_clicked()
{
    sendMessage(ChatMessage);
}

// 退出按钮
void Widget::on_exitButton_clicked()
{
    close();
}

// 关闭时发送离开信息
void Widget::closeEvent(QCloseEvent *e)
{
    sendMessage(LeftParticipant);
    QWidget::closeEvent(e);
}

// 回车发送
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->messageTextEdit) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if (k->key() == Qt::Key_Return) {
                on_sendButton_clicked();
                return true;
            }

        }
    }
    return QWidget::eventFilter(watched,event);
}

// 保存聊天记录
void Widget::on_saveButton_clicked()
{
    if(ui->messageTextBrowser->document()->isEmpty()) {
        QMessageBox::warning(0,tr("警告"),
                             tr("聊天记录为空,无法保存!"),QMessageBox::Ok);
    } else {
        QString fileName  = QFileDialog::getSaveFileName(this,tr("保存聊天记录"),tr("聊天记录"),
                                                         tr("文本(*.txt);All File(* . *)"));
        qDebug() << "保存聊天记录" << fileName;
    }
}


void Widget::on_clearButton_clicked()
{
    ui->messageTextBrowser->clear();
}

