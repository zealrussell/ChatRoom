#include "chatthread.h"

ChatThread::ChatThread():
    isBeCalled(false),
    localPort(0),
    peerPort(0),
    localName(""),
    peerName("")
{

}

ChatThread::~ChatThread()
{

}

void ChatThread::stop()
{
    if(chat != nullptr){
        chat->close();
        chat->deleteLater();
    }
}

quint16 ChatThread::getLocalPort()
{
    if (chat != nullptr)
        localPort =  chat->getLocalPort();
    return localPort;
}


void ChatThread::run()
{
    chat = new Chat(localName, peerName, peerPort);
    if (isBeCalled)
        chat->connectToChat();
    chat->show();
}

void ChatThread::setPeerName(const QString &newPeerName)
{
    peerName = newPeerName;
}

void ChatThread::setLocalName(const QString &newLocalName)
{
    localName = newLocalName;
}

void ChatThread::setPeerPort(quint16 newPeerPort)
{
    peerPort = newPeerPort;
}

void ChatThread::setLocalPort(quint16 newLocalPort)
{
    localPort = newLocalPort;
}

void ChatThread::setIsBeCalled(bool newIsBeCalled)
{
    isBeCalled = newIsBeCalled;
}
