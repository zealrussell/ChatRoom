/********************************************************************************
** Form generated from reading UI file 'fileserver.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILESERVER_H
#define UI_FILESERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FileServer
{
public:
    QLabel *label;
    QProgressBar *progressBar;
    QPushButton *fileOpenBtn;
    QPushButton *fileSendBtn;
    QPushButton *fileCloseBtn;

    void setupUi(QDialog *FileServer)
    {
        if (FileServer->objectName().isEmpty())
            FileServer->setObjectName(QString::fromUtf8("FileServer"));
        FileServer->resize(500, 200);
        label = new QLabel(FileServer);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(120, 20, 261, 50));
        progressBar = new QProgressBar(FileServer);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(50, 80, 401, 23));
        progressBar->setValue(24);
        fileOpenBtn = new QPushButton(FileServer);
        fileOpenBtn->setObjectName(QString::fromUtf8("fileOpenBtn"));
        fileOpenBtn->setGeometry(QRect(30, 150, 93, 28));
        fileSendBtn = new QPushButton(FileServer);
        fileSendBtn->setObjectName(QString::fromUtf8("fileSendBtn"));
        fileSendBtn->setGeometry(QRect(200, 150, 93, 28));
        fileCloseBtn = new QPushButton(FileServer);
        fileCloseBtn->setObjectName(QString::fromUtf8("fileCloseBtn"));
        fileCloseBtn->setGeometry(QRect(360, 150, 93, 28));

        retranslateUi(FileServer);

        QMetaObject::connectSlotsByName(FileServer);
    } // setupUi

    void retranslateUi(QDialog *FileServer)
    {
        FileServer->setWindowTitle(QApplication::translate("FileServer", "Dialog", nullptr));
        label->setText(QApplication::translate("FileServer", "\350\257\267\351\200\211\346\213\251\350\246\201\344\274\240\350\276\223\347\232\204\346\226\207\344\273\266\357\274\201", nullptr));
        fileOpenBtn->setText(QApplication::translate("FileServer", "\346\211\223\345\274\200", nullptr));
        fileSendBtn->setText(QApplication::translate("FileServer", "\345\217\221\351\200\201", nullptr));
        fileCloseBtn->setText(QApplication::translate("FileServer", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileServer: public Ui_FileServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILESERVER_H
