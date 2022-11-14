#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1",8888);
    if (socket->waitForConnected()) {
        qDebug() << "建立了连接";
        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::revMessage);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMessage()
{
     QByteArray data = ui->textEdit->toPlainText().toUtf8();
     qDebug() << "客户端：" << data;
     socket->write(data);
}

void MainWindow::revMessage()
{
    QByteArray data;

    data = socket->readAll();
    ui->textBrowser->append(data);

}



void MainWindow::on_pushButton_clicked()
{

    sendMessage();
    ui->textEdit->clear();
}

