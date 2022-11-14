#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QFile>
#include <QFileInfo>

class FileWorker : public QObject
{
   Q_OBJECT
public:
    explicit FileWorker(QTcpSocket *tcp = nullptr, QString filename = "", QObject *parent = nullptr);
    ~FileWorker();


    void closeSocket();

signals:
    void signalConnectOK();
    void signalDisconnectOK();
    void signalSendOver();
    void signalOpenFileFail();
    void signalFilePercent(int percent);

public slots:
    void slotSendFile();
private:
    QTcpServer *server;
    QTcpSocket *socket;
    QString fileName;
    quint16 localPort;
    quint16 peerPort;
};

#endif // FILEWORKER_H
