/********************************************************************************
** Form generated from reading UI file 'chat.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Chat
{
public:
    QTextBrowser *messageTextBrowser;
    QTextEdit *messageTextEdit;
    QPushButton *closeButton;
    QPushButton *sendButton;
    QLabel *label;
    QLabel *stateLabel;
    QPushButton *openButton;
    QComboBox *portBox;

    void setupUi(QDialog *Chat)
    {
        if (Chat->objectName().isEmpty())
            Chat->setObjectName(QString::fromUtf8("Chat"));
        Chat->resize(600, 500);
        messageTextBrowser = new QTextBrowser(Chat);
        messageTextBrowser->setObjectName(QString::fromUtf8("messageTextBrowser"));
        messageTextBrowser->setGeometry(QRect(10, 90, 571, 251));
        messageTextEdit = new QTextEdit(Chat);
        messageTextEdit->setObjectName(QString::fromUtf8("messageTextEdit"));
        messageTextEdit->setGeometry(QRect(10, 360, 571, 87));
        closeButton = new QPushButton(Chat);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(440, 460, 93, 28));
        sendButton = new QPushButton(Chat);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        sendButton->setGeometry(QRect(50, 460, 93, 28));
        label = new QLabel(Chat);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 40, 431, 31));
        stateLabel = new QLabel(Chat);
        stateLabel->setObjectName(QString::fromUtf8("stateLabel"));
        stateLabel->setGeometry(QRect(470, 50, 91, 20));
        openButton = new QPushButton(Chat);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        openButton->setGeometry(QRect(460, 10, 93, 28));
        portBox = new QComboBox(Chat);
        portBox->setObjectName(QString::fromUtf8("portBox"));
        portBox->setGeometry(QRect(20, 10, 87, 22));

        retranslateUi(Chat);

        QMetaObject::connectSlotsByName(Chat);
    } // setupUi

    void retranslateUi(QDialog *Chat)
    {
        Chat->setWindowTitle(QApplication::translate("Chat", "Dialog", nullptr));
        closeButton->setText(QApplication::translate("Chat", "\345\205\263\351\227\255", nullptr));
        sendButton->setText(QApplication::translate("Chat", "\345\217\221\351\200\201", nullptr));
        label->setText(QApplication::translate("Chat", "TextLabel", nullptr));
        stateLabel->setText(QApplication::translate("Chat", "\347\255\211\345\276\205\350\277\236\346\216\245\343\200\202\343\200\202", nullptr));
        openButton->setText(QApplication::translate("Chat", "\346\211\223\345\274\200\347\253\257\345\217\243", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Chat: public Ui_Chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
