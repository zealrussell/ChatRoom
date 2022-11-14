#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QTime>
#include <QStringList>
#include <QMap>
#include "recvmsg.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();
    void slotBroadcastMsg(QTcpSocket *socket, ChatMessage msg);
    void slotBroadcastUser();
private:
    Ui::Server *ui;

    // QTcpSocket *socket;
    QTcpServer *server;
    QTime time;
    int count;
    QList<QTcpSocket *> socketList;
    QMap<QTcpSocket *, QString> socketMap;
    ChatMessage numMessage;
    ChatMessage chatMessage;
    // 数据库操作
    QSqlDatabase db;
    QSqlQuery sql;
};
#endif // SERVER_H
