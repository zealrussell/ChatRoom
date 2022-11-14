QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#_BUILDING_USER，通过它可以知道项目是否正在编译
DEFINES += _BUILDING_APP

#包含模块的pri文件，一些宏定义使用中需要用到
include($$PWD/QxOrm/QxOrm.pri)

#添加源代码的头文件路劲，后面代码包含头文件的时候就可以使用相对路径了
INCLUDEPATH += $$PWD/QxOrm/include

#添加编译好的动态库，QxOrm源码编译的动态库在QxOrm/lib目录中，最好编译debug、release两个版本
LIBS += -L$$PWD/QxOrm/lib
CONFIG(debug, debug|release) {
LIBS += -lQxOrmd
} else {
LIBS +=  -lQxOrm
}


SOURCES += \
    chat.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    chat.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
