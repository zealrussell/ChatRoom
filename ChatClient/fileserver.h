#ifndef FILESERVER_H
#define FILESERVER_H

#include <QDialog>
#include <QTcpServer>
#include <QFile>
#include <QTcpSocket>
#include <QTime>
#include <QFileDialog>
#include <QString>
#include <QCloseEvent>
#include <QWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QThread>

#include "fileworker.h"

namespace Ui {
class FileServer;
}

class FileServer : public QDialog
{
    Q_OBJECT

public:
    explicit FileServer(QWidget *parent = nullptr);
    ~FileServer();

    void initServer(quint16 udpPort = 0);
    void refused();
    void setTargePort(quint16 port);
    quint16 getTcpPort();
protected:
    void closeEvent(QCloseEvent *);

signals:
    void signalSendFileName(QString, quint16, quint16);
    void signalSendNameToWorker(QString filename);
    void signalBeginSend();
private slots:

    void slotSendFile();
    void slotUpdateClientProgress(qint64);

    void on_fileOpenBtn_clicked();
    void on_fileSendBtn_clicked();
    void on_fileCloseBtn_clicked();

private:
    Ui::FileServer *ui;

    QTcpServer *tcpServer;
    QTcpSocket *clientConnection;
    quint16 targetPort;
    quint16 tcpPort;
    QFile *localFile ;

    qint64 payloadSize ;
    qint64 TotalBytes ;
    qint64 bytesWritten ;
    qint64 bytestoWrite;

    QString theFileName;
    QString fileName;


    QTime time;
    QByteArray outBlock;

};

#endif // FILESERVER_H
