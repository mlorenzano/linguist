#-------------------------------------------------
#
# Project created by QtCreator 2018-07-02T16:28:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ElcoLinguist
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

OBJECTS_DIR = $${OUT_PWD}/obj
MOC_DIR = $${OUT_PWD}/obj
RCC_DIR = $${OUT_PWD}/obj
UI_DIR = $${OUT_PWD}/obj

INCLUDEPATH += $$PWD/src \
                $$PWD/include \
                $$PWD/ui \

SOURCES += \
    src/aboutdialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingsdialog.cpp

HEADERS += \
    include/aboutdialog.h \
    include/mainwindow.h \
    include/settingsdialog.h

FORMS += \
    ui/aboutdialog.ui \
    ui/mainwindow.ui \
    ui/settingsdialog.ui

RESOURCES += \
    resources.qrc
