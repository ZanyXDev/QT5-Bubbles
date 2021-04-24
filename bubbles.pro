!versionAtLeast(QT_VERSION, 5.10.0):error("Requires Qt version 5.10.0 or greater.")

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(gitversion.pri)

SOURCES += \
    src/main.cpp\
    src/mainwidget.cpp \
    src/bubbleitem.cpp

HEADERS  += \
    src/mainwidget.h \
    src/bubbleitem.h

RESOURCES += \
    resources.qrc
