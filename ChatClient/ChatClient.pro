QT       += core gui network sql serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chat.cpp \
    chatthread.cpp \
    fileclient.cpp \
    fileserver.cpp \
    fileworker.cpp \
    main.cpp \
    utils.cpp \
    widget.cpp

HEADERS += \
    chat.h \
    chatthread.h \
    fileclient.h \
    fileserver.h \
    fileworker.h \
    utils.h \
    widget.h

FORMS += \
    chat.ui \
    fileclient.ui \
    fileserver.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
