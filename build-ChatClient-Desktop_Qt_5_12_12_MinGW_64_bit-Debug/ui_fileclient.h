/********************************************************************************
** Form generated from reading UI file 'fileclient.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILECLIENT_H
#define UI_FILECLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FileClient
{
public:
    QPushButton *fileCancelBtn;
    QPushButton *fileCloseBtn;
    QProgressBar *progressBar;
    QLabel *label;

    void setupUi(QDialog *FileClient)
    {
        if (FileClient->objectName().isEmpty())
            FileClient->setObjectName(QString::fromUtf8("FileClient"));
        FileClient->resize(500, 200);
        fileCancelBtn = new QPushButton(FileClient);
        fileCancelBtn->setObjectName(QString::fromUtf8("fileCancelBtn"));
        fileCancelBtn->setGeometry(QRect(60, 160, 93, 28));
        fileCloseBtn = new QPushButton(FileClient);
        fileCloseBtn->setObjectName(QString::fromUtf8("fileCloseBtn"));
        fileCloseBtn->setGeometry(QRect(290, 160, 93, 28));
        progressBar = new QProgressBar(FileClient);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(50, 80, 421, 41));
        progressBar->setValue(24);
        label = new QLabel(FileClient);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, 20, 150, 50));

        retranslateUi(FileClient);

        QMetaObject::connectSlotsByName(FileClient);
    } // setupUi

    void retranslateUi(QDialog *FileClient)
    {
        FileClient->setWindowTitle(QApplication::translate("FileClient", "Dialog", nullptr));
        fileCancelBtn->setText(QApplication::translate("FileClient", "\345\217\226\346\266\210", nullptr));
        fileCloseBtn->setText(QApplication::translate("FileClient", "\345\205\263\351\227\255", nullptr));
        label->setText(QApplication::translate("FileClient", "\350\257\267\346\216\245\346\224\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileClient: public Ui_FileClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILECLIENT_H
