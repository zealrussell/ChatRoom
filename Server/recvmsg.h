#ifndef RECVMSG_H
#define RECVMSG_H

#include <QThread>
#include <QTcpSocket>
#include <QTime>

class RecvMsg : public QThread
{
    Q_OBJECT
public:
    explicit RecvMsg(QTcpSocket* tcp,QObject *parent = nullptr);

protected:
    void run() override;

private:
    QTcpSocket *socket;
signals:
    void over(QString msg);
};

#endif // RECVMSG_H
