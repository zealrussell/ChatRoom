#include "chat.h"
#include "ui_chat.h"

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
#include<QFileDialog>
#include<QColorDialog>
#include<QHostAddress>
#include<QFontComboBox>
#include<QTextEdit>

#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>     // 提供系统中存在的串口信息

Chat::Chat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    this->setWindowTitle("私聊界面");
}

Chat::~Chat()
{
    delete ui;
}

Chat::Chat(QString myname,QString peername, quint16 peerport):
    localName(myname),
    peerName(peername),
    peerPort(peerport),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    this->setWindowTitle(localName);
    ui->label->setText(tr("与%1私聊中 对方的端口：%2").arg(peerName).arg(peerPort));
    socket = new QUdpSocket(this);
    socket->bind(0, QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    localPort = socket->localPort();
    localAddress = socket->localAddress().toString();

    qDebug() << "私聊：本地" << localName << "端口" << localPort;
    initSerialPort();
    connect(socket, &QUdpSocket::readyRead, this, &Chat::slotReceveMessageFromNet);


}

quint16 Chat::getLocalPort()
{
    return localPort;
}

void Chat::connectToChat()
{
    if (peerPort == 0) return;
    // socket->connectToHost(QHostAddress::LocalHost, peerPort);
    sendMessageToNet(NewParticipant);
}

// 初始化端口
void Chat::initSerialPort()
{
    port = new QSerialPort;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
        {
            port->setPort(info);                      // 在对象中设置串口
            if(port->open(QIODevice::ReadWrite))      // 以读写方式打开串口
            {
                ui->portBox->addItem(info.portName());  // 添加计算机中的端口
                port->close();                        // 关闭
            } else
            {
                qDebug() << "串口打开失败，请重试";
            }
        }

    port->setBaudRate(QSerialPort::Baud57600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    refreshSerialPort(0);
    connect(port, &QSerialPort::readyRead, this, &Chat::slotReceveMessageFromSerialPort);
}

// 刷新端口
void Chat::refreshSerialPort(int index)
{
    QStringList portNameList;                                        // 存储所有串口名
    if(index != 0)
    {
        port->setPortName(ui->portBox->currentText());             //设置串口号
    }
    else
    {
        ui->portBox->clear();                                        //关闭串口号
        ui->portBox->addItem("刷新");                                //添加刷新
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) //添加新串口
        {
            portNameList.append(info.portName());
        }
        ui->portBox->addItems(portNameList);
        ui->portBox->setCurrentIndex(1);                             // 当前串口号为COM1
        port->setPortName(ui->portBox->currentText());             //设置串口号
   }
}

// 网络协议发送
void Chat::sendMessageToNet(MessageType type)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    // 写入用户名
    out << type;
    out << localName << localAddress << localPort;
    switch(type) {
        case ChatMessage:
            if(ui->messageTextEdit->toPlainText() == "") {
                QMessageBox::warning(0,tr("警告"),
                                     tr("发送内容不能为空"),QMessageBox::Ok);
                return ;
            }
            // 写入 地址、消息内容
            out << getChatMessage();
            // 发送后移动到最下方
            ui->messageTextBrowser->verticalScrollBar()
                    ->setValue(ui->messageTextBrowser
                               ->verticalScrollBar()->maximum());
            socket->writeDatagram(data, data.length(), QHostAddress::LocalHost, peerPort);
            break;

        case NewParticipant:
            qDebug() << "私聊：发送新用户";
            socket->writeDatagram(data, data.length(), QHostAddress::LocalHost, peerPort);
            break;
        case LeftParticipant:
            socket->writeDatagram(data, data.length(), QHostAddress::LocalHost, peerPort);
            break;
    }
}

// 串口发送
void Chat::sendMessageToSerialPort(MessageType type)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    qDebug() << "串口发送：" << type;
    // 写入用户名
    out << type;
    out << localName << localAddress << localPort;
    switch(type) {
        case ChatMessage:
            if(ui->messageTextEdit->toPlainText() == "") {
                QMessageBox::warning(0,tr("警告"),
                                     tr("发送内容不能为空"),QMessageBox::Ok);
                return ;
            }
            // 写入 地址、消息内容
            out << getChatMessage().toLocal8Bit();
            // 发送后移动到最下方
            ui->messageTextBrowser->verticalScrollBar()
                    ->setValue(ui->messageTextBrowser
                               ->verticalScrollBar()->maximum());
            break;

        case NewParticipant:
            qDebug() << "私聊：发送新用户";
            break;
        case LeftParticipant:
            break;
    }
    port->write(data);
}

QString Chat::getChatMessage()
{
    QString msg = ui->messageTextEdit->toHtml();
    ui->messageTextEdit->setFocus();
    return msg;
}

// 网络协议接收数据
void Chat::slotReceveMessageFromNet()
{
    while(socket ->hasPendingDatagrams())
    {
        // qDebug() << "已链接上";
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size());

        // 反序列化
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        QString userName, userIp;
        quint16 userPort;

        in >> messageType;
        in >> userName >> userIp >> userPort;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        switch (messageType) {
            case ChatMessage:{
                QString message;
                qDebug() << "私聊net：接收消息" << message;
                in >> message;
                if (message.isNull() || message.isEmpty() ) break;

                ui->stateLabel->setText(tr("在线"));
                ui->messageTextBrowser->setTextColor(Qt::blue);
                ui->messageTextBrowser->setCurrentFont(QFont("Times New Roman",12));
                ui->messageTextBrowser->append("[ " + userName + " ]" + time);
                ui->messageTextBrowser->append(message);

                break;
            }
            case NewParticipant:
                qDebug() << "私聊net：用户加入,端口" << userPort;
                if (peerPort == 0) peerPort = userPort;
                ui->label->setText(tr("与%1私聊中 对方的端口：%2").arg(peerName).arg(peerPort));
                ui->stateLabel->setText(tr("在线"));
                break;
            case LeftParticipant:
                qDebug() << "私聊net：用户退出" << userName;
                ui->stateLabel->setText(tr("已离线"));
                break;

        }

    }
}

// 从串口接收数据
void Chat::slotReceveMessageFromSerialPort()
{
    while(port->bytesAvailable()) {
        QByteArray datagram;
        datagram.resize(port->bytesAvailable());
        port->read(datagram.data(),datagram.size());
        // 反序列化
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        QString userName, userIp;
        quint16 userPort;

        in >> messageType;
        in >> userName >> userIp >> userPort;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        switch (messageType) {
            case ChatMessage:{
                QByteArray message;
                qDebug() << "私聊port：接收消息";
                in >> message;
                if (message.isNull() || message.isEmpty() ) break;

                ui->messageTextBrowser->setTextColor(Qt::blue);
                ui->messageTextBrowser->setCurrentFont(QFont("Times New Roman",12));
                ui->messageTextBrowser->append("[ " + userName + " ]" + time);
                ui->messageTextBrowser->append(QString::fromLocal8Bit(message));

                break;
            }
            case NewParticipant:
                qDebug() << "私聊port：用户加入,端口" << userPort;
                if (peerPort == 0) peerPort = userPort;
                ui->label->setText(tr("与%1私聊中 对方的端口：%2").arg(peerName).arg(peerPort));
                ui->stateLabel->setText(tr("在线"));
                break;
            case LeftParticipant:
                qDebug() << "私聊port：用户退出" << userName;
                ui->stateLabel->setText(tr("已离线"));
                break;

        }
    }
}



void Chat::on_sendButton_clicked()
{
    if (portFlag)
        sendMessageToSerialPort(ChatMessage);
    else
        sendMessageToNet(ChatMessage);
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString message = getChatMessage();
    ui->messageTextBrowser->setTextColor(Qt::blue);
    ui->messageTextBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->messageTextBrowser->append("[ " + localName + " ]" + time);
    ui->messageTextBrowser->append(message);
    ui->messageTextEdit->clear();

}


void Chat::on_closeButton_clicked()
{
    sendMessageToNet(LeftParticipant);
    port->close();
    close();
}

// 打开端口
void Chat::on_openButton_clicked()
{
    if (port->isOpen()) {
        port->clear();
        port->close();
        ui->openButton->setText("打开");
        portFlag = false;
    } else {
        port->setPortName(ui->portBox->currentText());
        if (!port->open(QIODevice::ReadWrite)) {
            QMessageBox::warning(this,tr("提示"),tr("串口打开失败!"),QMessageBox::Ok);
            return;
        } else {
            ui->openButton->setText("关闭");
            portFlag = true;
        }
    }
}

