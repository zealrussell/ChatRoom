#include "chatroom.h"
#include "ui_chatroom.h"

ChatRoom::ChatRoom(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatRoom)
{
    ui->setupUi(this);
    time = new QTime;
    socket = new QTcpSocket(this);
    this->setWindowTitle("Client");

    username = "";
    connect(socket, &QTcpSocket::connected, this, [=](){
        connectState = true;
        ui->connectBtn->setText("断开");
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
    MyMessage sendMessage;
    sendMessage.type = 1;
    sendMessage.username = username;
    sendMessage.message = ui->chatText->toPlainText();
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_0);
    stream << sendMessage.type << sendMessage.username << sendMessage.message;
    qDebug() << "发送 " << block;
    if(socket->isOpen() && socket->isValid() ) {
        socket->write(block, block.size());
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
        connect(socket, &QTcpSocket::readyRead, this, &ChatRoom::slotRevMessage);
        registerUsername();
    } else {                                            // 客户端已经连接
        socket->close();                   // 关闭套接字，此时会发送disconnected信号
        connectState = false;
        ui->connectBtn->setText("连接");
        count = 0;
        usernameList.clear();
        QStringListModel *listmodel = new QStringListModel(usernameList);
        ui->usernameListView->setModel(listmodel);
        ui->onlineLable->setText(QString("在线人数: %1").arg(count));
        QMessageBox::information(this, "连接状态", "已成功断开连接！");
        // connect(socket, &QTcpSocket::disconnected, this, [=](){});
    }
}



// 接收信息
void ChatRoom::slotRevMessage() {
    MyMessage revMessage;
    qint64 size = socket->bytesAvailable();
    int mysize = static_cast<int>(size);
    QByteArray block = QByteArray(mysize,0);
    socket->read(block.data(), size);
    QDataStream out(&block, QIODevice::ReadOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out >> revMessage.type >> revMessage.username >> revMessage.message;
    qDebug() << "接收：" << block;
    qDebug() << revMessage.message;
    int type = revMessage.type;
    if (type == 1) {
        ui->allText->append(time->currentTime().toString());
        ui->allText->append(revMessage.username + ": " + revMessage.message + "\n");
    } else if (type == 2) {
        count = revMessage.message.toUInt();
        ui->onlineLable->setText(QString("在线人数: %1").arg(count));
        usernameList = revMessage.username.split(",");
        for(QString &name : usernameList) {
            if(name.compare(username) == 0)
                usernameList.removeOne(name);
        }
        usernameList.insert(0, username);

        //设置模型到listview上
        QStringListModel *listmodel = new QStringListModel(usernameList);
        ui->usernameListView->setModel(listmodel);

    }
}

void ChatRoom::registerUsername() {

    MyMessage sendMessage;
    sendMessage.type = 2;
    sendMessage.username = username;
    sendMessage.message = username;
    qDebug() << "注册" << username;
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_0);
    stream << sendMessage.type << sendMessage.username << sendMessage.message;
    if(socket->isOpen() && socket->isValid() ) {
        socket->write(block, block.size());
    }
}

//void ChatRoom::sendUsername() {
//    sendMessage.type = 3;
//    sendMessage.username = username;
//    sendMessage.message = username;
//    QByteArray block;
//    QDataStream stream(&block, QIODevice::WriteOnly);
//    stream.setVersion(QDataStream::Qt_5_0);
//    stream << sendMessage.type << sendMessage.username << sendMessage.message;
//    qDebug() << "发送 " << block;
//    if(socket->isOpen() && socket->isValid() ) {
//        socket->write(block, block.size());
//        ui->chatText->clear();
//    }
//}




