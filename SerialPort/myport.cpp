#include "myport.h"
#include "ui_myport.h"

MyPort::MyPort(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyPort)
{
    ui->setupUi(this);
    init();
    connect(port, &QSerialPort::readyRead, this, &MyPort::receveMessage);


}

MyPort::~MyPort()
{
    delete ui;
}

void MyPort::init()
{
    port = new QSerialPort;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
        {
            port->setPort(info);                      // 在对象中设置串口
            if(port->open(QIODevice::ReadWrite))      // 以读写方式打开串口
            {
                ui->portBox->addItem(info.portName());  // 添加计算机中的端口
                port->close();                        // 关闭
            } else
            {
                qDebug() << "串口打开失败，请重试";
            }
        }

    port->setBaudRate(QSerialPort::Baud57600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    refreshSerialPort(0);
}

void MyPort::sendMessage()
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    // 写入用户名
    out << Chat;
    out << ui->textEdit->toPlainText().toLocal8Bit();

    ui->textEdit->clear();
    port->write(data);
}

void MyPort::receveMessage()
{
    // 接收数据,使用read () / readLine () / readAll ()

    QByteArray datagram;
    datagram.resize(port->bytesAvailable());
    port->read(datagram.data(),datagram.size());
    // 反序列化
    QDataStream in(&datagram,QIODevice::ReadOnly);
    int type;
    QByteArray msg;
    in >> type >> msg;
    if (type == Chat) qDebug() << "类型" << type;
                      // 读取数据
    qDebug() << "数据：" << msg;
    ui->textBrowser->append(QString::fromLocal8Bit(msg));

}

// 刷新串口
void MyPort::refreshSerialPort(int index)
{
    QStringList portNameList;                                        // 存储所有串口名
    if(index != 0)
    {
        port->setPortName(ui->portBox->currentText());             //设置串口号
    }
    else
    {
        ui->portBox->clear();                                        //关闭串口号
        ui->portBox->addItem("刷新");                                //添加刷新
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) //添加新串口
        {
            portNameList.append(info.portName());
        }
        ui->portBox->addItems(portNameList);
        ui->portBox->setCurrentIndex(1);                             // 当前串口号为COM1
        port->setPortName(ui->portBox->currentText());             //设置串口号
   }
}

void MyPort::on_pushButton_clicked()
{
    sendMessage();
}



void MyPort::on_openBtn_clicked()
{
    if (port->isOpen()) {
        port->clear();
        port->close();
        ui->openBtn->setText("打开");
    } else {
        port->setPortName(ui->portBox->currentText());
        port->open(QIODevice::ReadWrite);
        ui->openBtn->setText("关闭");

    }
}

