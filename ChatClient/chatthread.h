#ifndef CHATTHREAD_H
#define CHATTHREAD_H

#include <QThread>
#include <QDebug>
#include "chat.h"

class ChatThread : public QThread
{
    Q_OBJECT
public:
    explicit  ChatThread();
    ~ChatThread();

    void stop();

    quint16 getLocalPort();
    void setIsBeCalled(bool newIsBeCalled);

    void setLocalPort(quint16 newLocalPort);

    void setPeerPort(quint16 newPeerPort);

    void setLocalName(const QString &newLocalName);

    void setPeerName(const QString &newPeerName);

protected:
    void run();

private:
    Chat *chat;
    bool isBeCalled;
    quint16 localPort;
    quint16 peerPort;
    QString localName;
    QString peerName;

};

#endif // CHATTHREAD_H
