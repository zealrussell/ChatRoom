#ifndef MYPORT_H
#define MYPORT_H

#include <QMainWindow>
#include <QDebug>

// 引入串口通信的两个头文件(第一步)
#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>     // 提供系统中存在的串口信息

QT_BEGIN_NAMESPACE
namespace Ui { class MyPort; }
QT_END_NAMESPACE

enum MessageType {
    Chat,
    File
};

class MyPort : public QMainWindow
{
    Q_OBJECT
public:
    MyPort(QWidget *parent = nullptr);
    ~MyPort();

private slots:
    void on_pushButton_clicked();

    void on_openBtn_clicked();

private:
    Ui::MyPort *ui;
    QSerialPort *port;


    void init();
    void sendMessage();
    void receveMessage();
    void refreshSerialPort(int index);
};
#endif // MYPORT_H
