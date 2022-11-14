#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    initDb();
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, 8888);

    connect(socket, &QUdpSocket::readyRead, this, &Widget::slotReceveMessage);
}

Widget::~Widget()
{
    portToNameMap.clear();
    db.close();
    delete ui;
}

void Widget::initDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("chatroom.db");
    if(!db.open()){
            //如果数据库打开失败
            QMessageBox::warning(this,"错误","打开数据库错误");
            return;
    }
    if (!db.tables().contains(TABLENAME)){
        QSqlQuery query;
        QString sql = QString("CREATE TABLE %1 (id int primary key autoincrement, username varchar(20), chatmessage varchar(1000));").arg(TABLENAME);
        query.prepare(sql);
        if( !query.exec()){
            qDebug() << "！！错误：" << query.lastError();
        }
    }

}


// 接收消息，处理后统一传送
void Widget::slotReceveMessage() {
    while(socket ->hasPendingDatagrams())
    {
        // qDebug() << "已链接上";
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size());

        // 反序列化
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        QString userName, ip;
        quint16 peerPort;

        in >> messageType;
        in >> userName >> ip >> peerPort;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        switch (messageType) {
            case ChatMessage:{
                qDebug() << "服务器：接收消息";
                QString message;
                in >> message;
                if (message.isNull() || message.isEmpty() ) break;
                ui->messageTextBrowser->setTextColor(Qt::blue);
                ui->messageTextBrowser->setCurrentFont(QFont("Times New Roman",12));
                ui->messageTextBrowser->append("[ " + userName + " ]" + time);
                ui->messageTextBrowser->append(message);

                qDebug() << message;
                QSqlQuery query;
                QString sql = QString("INSERT INTO %1 VALUES (null, '%2', '%3');").arg(TABLENAME).arg(userName).arg(message);
                if( !query.exec(sql)){
                    qDebug() << "！！错误：" << query.lastError();
                }


                broadcastData(datagram);
                break;
            }
            case NewParticipant:
                qDebug() << "服务器：用户加入";
                newParticipant(userName, ip, peerPort);
                updateUerList(NewParticipant, userName, ip, peerPort);
                break;
            case LeftParticipant:
                qDebug() << "服务器：用户退出" << userName;
                leftParticipant(userName,peerPort,time);
                updateUerList(LeftParticipant, userName, ip, peerPort);
                break;

        }

    }
}


void Widget::newParticipant(QString userName, QString ipAddress, quint16 peerPort)
{
    if (portToNameMap.find(peerPort) != portToNameMap.end()) return;
    qDebug() <<"用户：" << userName << " :" << peerPort << "登录";
    portToNameMap.insert(peerPort, userName);

    QTableWidgetItem *user = new QTableWidgetItem(userName);
    QTableWidgetItem *ip = new QTableWidgetItem(ipAddress);
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
}

void Widget::leftParticipant(QString userName, quint16 peerPort,QString time)
{
    // qDebug() <<"用户：" << userName << " :" << peerPort << "退出";
    if (!portToNameMap.contains(peerPort)) return;

    portToNameMap.remove(peerPort);
    // 找到对应的行号
    int rowNum = ui->userTableWidget->findItems(QString::number(peerPort),Qt::MatchExactly).at(0)->row();

    ui->userTableWidget->removeRow(rowNum);

    ui->messageTextBrowser->setTextColor(Qt::gray);
    ui->messageTextBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->messageTextBrowser->append(tr("%1:%2于%3离开!").arg(userName).arg(peerPort).arg(time));

    ui->userNumberLabel->setText(tr("在线人数:%1")
                                 .arg(ui->userTableWidget->rowCount()));
}

void Widget::updateUerList(MessageType type, QString userName,QString ip, quint16 port)
{
    qDebug() << "更新列表：" << portToNameMap;
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    // 写入用户名
    out << type;
    out << userName << ip << port;
    out << portToNameMap;
    broadcastData(data);
}

void Widget::broadcastData(const QByteArray &data)
{
    auto it = portToNameMap.constBegin();
    while (it != portToNameMap.constEnd()) {
       socket->writeDatagram(data, data.length(), QHostAddress::LocalHost, it.key());
       it++;
    }
}


