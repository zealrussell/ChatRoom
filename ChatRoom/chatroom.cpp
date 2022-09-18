#include "chatroom.h"
#include "ui_chatroom.h"

ChatRoom::ChatRoom(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatRoom)
{
    socket = new QTcpSocket(this);
    ui->setupUi(this);

    this->setWindowTitle("Client");

    username = "";

    connect(socket, &QTcpSocket::connected, this, [=](){
        QMessageBox::information(this, "连接", username + "连接成功!");
    });
};

ChatRoom::~ChatRoom()
{
    socket->close();
    delete ui;
}

// 连接按钮
void ChatRoom::on_connectBtn_clicked()
{
    connectToServer();
}

// 清除按钮
void ChatRoom::on_clearBtn_clicked()
{
    ui->chatText->clear();
}

// 发送按钮
void ChatRoom::on_sendBtn_clicked()
{
    QString text = username + ": " + ui->chatText->toPlainText();
    if(socket->isOpen() && socket->isValid() ) {
        socket->write(text.toUtf8());
        ui->chatText->clear();
    }
}

// 连接或断开
void ChatRoom::connectToServer() {
    QString ipStr = "127.0.0.1";                        // 界面显示的地址
    quint16 currentPort = 8848;                         // 界面显示的当前端口
    if(!connectState) {                                 // 客户端还未连接服务端
        username = ui->usernameText->text();
        if (username == NULL || username == "") {
            QMessageBox::information(this, "用户名错误", "请输入用户名!");
            return;
        }

        socket->connectToHost(ipStr, currentPort);      // 连接服务端
        if(socket->waitForConnected()) {                 // 等待连接成功
            connectState = true;
            ui->connectBtn->setText("断开");
            connect(socket, &QTcpSocket::readyRead, this, &ChatRoom::readMessage);

        } else {
            QMessageBox::information(this, "连接状态", "服务器未开启");
        }
    } else {                                            // 客户端已经连接
        connectState = false;
        ui->connectBtn->setText("连接");
        socket->disconnectFromHost();                   // 关闭套接字，此时会发送disconnected信号
        connect(socket, &QTcpSocket::disconnected, this, [=](){
            QMessageBox::information(this, "连接状态", "已成功断开连接！");
        });
    }
}

// 接收信息
void ChatRoom::readMessage() {
    while(socket->bytesAvailable() > 0) {
        QByteArray type = socket->readLine();
        QByteArray array = socket->readAll();

        ui->allText->append(array);
    }
}




