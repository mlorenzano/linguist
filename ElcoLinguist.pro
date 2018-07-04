#-------------------------------------------------
#
# Project created by QtCreator 2018-07-02T16:28:40
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ElcoLinguist
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

OBJECTS_DIR = $$PWD/obj
MOC_DIR = $$PWD/obj
RCC_DIR = $$PWD/obj
UI_DIR = $$PWD/obj


CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/libs-3rdparty/libCSV -llibCSVd
} else {
    LIBS += -L$$PWD/libs-3rdparty/libCSV -llibCSV
}

INCLUDEPATH += $$PWD/src \
               $$PWD/include \
               $$PWD/ui \
               $$PWD/libs-3rdparty/libCSV/shared

SOURCES += \
    src/aboutdialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingsdialog.cpp \

HEADERS += \
    include/aboutdialog.h \
    include/mainwindow.h \
    include/settingsdialog.h \
    libs-3rdparty/libCSV/shared/csv.h \
    libs-3rdparty/libCSV/shared/csv_global.h \

FORMS += \
    ui/aboutdialog.ui \
    ui/mainwindow.ui \
    ui/settingsdialog.ui

RESOURCES += \
    resources.qrc
