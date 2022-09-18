#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();
    void broadcastMsg(QString msg);
    void broadcastNum(int number);
private:
    Ui::Server *ui;
    QTcpSocket *socket;
    QTcpServer *server;
    int count;
    QList<QTcpSocket *> socketList;

signals:
    void userNum(int num);
};
#endif // SERVER_H
