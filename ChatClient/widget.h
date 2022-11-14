#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

#include "fileclient.h"
#include "fileserver.h"
#include "chat.h"
#include "chatthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:

    QString getIp();
    quint16 getFreePort();
    QString getUsername();
    QString getMessage();
    void sendMessage(MessageType type, quint16 targetPort = 0, quint16 tcpPort = 0);


    // void newParticipant(QString userName, QString ipAddress,quint16 port);
    void newParticipant(const QMap<quint16, QString> &map);
    void leftParticipant(const QMap<quint16, QString> &map, QString time);

    void receveFile(QString serverName,quint16 serverPort, quint16 targetPort,QString fileName);
    bool saveFile(const QString &fileName);
    void closeEvent(QCloseEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
    void showXchat(QString myName, QString peerName, quint16 udpPort);

private:
    QUdpSocket *socket;
    FileServer *server;
    quint16 localPort;
    quint16 serverPort;
    QString localAddress;
    QString localName;
    QString filename;
    Ui::Widget *ui;
    QMap<qint16, QString> portToNameMap;

private slots:
    void slotGetFile(QString name, quint16 targetPort, quint16 serverPort);
    void slotReceveMessage();

    void on_sendButton_clicked();
    void on_exitButton_clicked();
    void on_userTableWidget_doubleClicked(const QModelIndex &index);

    void on_saveButton_clicked();
    void on_clearButton_clicked();
    void on_sendFileButton_clicked();
};
#endif // WIDGET_H
