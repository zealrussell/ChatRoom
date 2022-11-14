#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QUdpSocket>
#include <QString>
#include <QMap>
#include <QDateTime>
#include <QMessageBox>

#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

enum MessageType{
    ChatMessage,
    NewParticipant,
    LeftParticipant,
    Refuse,
    FileName,
    Xchat
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void initDb();

    void newParticipant(QString userName, QString ipAddress, quint16 port);
    void leftParticipant(QString userName, quint16 port, QString time);
    void updateUerList(MessageType type, QString userName,QString ip, quint16 port);
    void broadcastData(const QByteArray &data);

private slots:

    void slotReceveMessage();

private:
    Ui::Widget *ui;
    QUdpSocket *socket;
    QMap<quint16, QString> portToNameMap;
    QSqlDatabase db;

    const QString TABLENAME = "message_table";

};
#endif // WIDGET_H
