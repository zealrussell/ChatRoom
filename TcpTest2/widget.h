#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QByteArray>
#include <QDebug>

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
    void newConnect();
    void revMessage(QTcpSocket *socket);
    void broadcast(QByteArray *data);
private:
    Ui::Widget *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QList<QTcpSocket *> socketList;

};
#endif // WIDGET_H
