#ifndef CHAT_H
#define CHAT_H

#include <QDialog>

#include<QUdpSocket>
#include<QTcpServer>
#include<QDataStream>
#include<QTime>
#include<QString>

#include"fileserver.h"
#include"fileclient.h"

#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>     // 提供系统中存在的串口信息


namespace Ui {
class Chat;
}

// 定义消息类型
enum MessageType{
    ChatMessage,
    NewParticipant,
    LeftParticipant,
    Refuse,
    FileName,
    Xchat
};

class Chat : public QDialog
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();
    Chat(QString myName,QString peerName, quint16 peerport = 0);

    quint16 getLocalPort();
    void connectToChat();


protected:
    void initSerialPort();
    void refreshSerialPort(int index);

    void sendMessageToNet(MessageType type);
    void sendMessageToSerialPort(MessageType type);
    QString getChatMessage();


private slots:

    void slotReceveMessageFromNet();
    void slotReceveMessageFromSerialPort();

    void on_sendButton_clicked();

    void on_closeButton_clicked();

    void on_openButton_clicked();

private:

    QSerialPort *port;
    bool portFlag;
    FileServer *server;
    QColor color;

    QUdpSocket *socket;
    QString localName, peerName, localAddress;
    quint16 localPort, peerPort;

    Ui::Chat *ui;

};

#endif // CHAT_H
