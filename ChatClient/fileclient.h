#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QDialog>
#include <QTcpServer>
#include <QFile>
#include <QTcpSocket>
#include <QTime>
#include <QFileDialog>
#include <QString>
#include <QCloseEvent>
#include <QHostAddress>

namespace Ui {
class FileClient;
}

class FileClient : public QDialog
{
    Q_OBJECT

public:
    explicit FileClient(QWidget *parent = nullptr, quint16 port = 6666);
    ~FileClient();

    void setFileName(QString fileName);
    void setServerTcpPort(quint16 port);
    void serServerUdpPort(quint16 port);
    void setHostAddress(QHostAddress address);

    void newConnect();

    void closeEvent(QCloseEvent *);

    QFile *localFile;

private slots:

    void slotReceveFile();
    void slotDisplayError(QAbstractSocket::SocketError);



    void on_fileCancelBtn_clicked();
    void on_fileCloseBtn_clicked();

private:


    QTcpSocket *tcpClient;

    quint16 tcpPort;
    quint16 udpPort;
    QHostAddress hostAddress = QHostAddress::LocalHost;

    qint64 TotalBytes ;
    qint64 bytesReceived;
    qint64 fileNameSize ;
    qint64 blockSize;
    QString fileName;

    QTime time;
    QByteArray inBlock;

    Ui::FileClient *ui;
};

#endif // FILECLIENT_H
