QT       += core gui xml concurrent

CONFIG += c++1z


CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS += -O3
}

CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -O0
    QMAKE_CXXFLAGS -= -O1
    QMAKE_CXXFLAGS -= -O2
    QMAKE_CXXFLAGS -= -O3
    QMAKE_CXXFLAGS += --debug
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ElcoLinguist
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QTCSV_LIBRARY

include($$PWD/libs-3rdparty/qtcsv/qtcsv.pri)

INCLUDEPATH += \
    $$PWD/ElcoLinguist/src \
    $$PWD/ElcoLinguist/include \
    $$PWD/ElcoLinguist/ui

SOURCES += \
    $$PWD/ElcoLinguist/src/main.cpp \
    $$PWD/ElcoLinguist/src/aboutdialog.cpp \
    $$PWD/ElcoLinguist/src/mainwindow.cpp \
    $$PWD/ElcoLinguist/src/settingsdialog.cpp \
    $$PWD/ElcoLinguist/src/language.cpp \
    $$PWD/ElcoLinguist/src/key.cpp \
    $$PWD/ElcoLinguist/src/languagestablemanager.cpp \
    $$PWD/ElcoLinguist/src/messageitem.cpp \
    $$PWD/ElcoLinguist/src/languagelistdialog.cpp \
    $$PWD/ElcoLinguist/src/customitemdelegate.cpp \
    $$PWD/ElcoLinguist/src/filereader.cpp \
    $$PWD/ElcoLinguist/src/filewriter.cpp

HEADERS += \
    $$PWD/ElcoLinguist/include/aboutdialog.h \
    $$PWD/ElcoLinguist/include/mainwindow.h \
    $$PWD/ElcoLinguist/include/settingsdialog.h \
    $$PWD/ElcoLinguist/include/language.h \
    $$PWD/ElcoLinguist/include/key.h \
    $$PWD/ElcoLinguist/include/languagestablemanager.h \
    $$PWD/ElcoLinguist/include/languagetablemodel.h \
    $$PWD/ElcoLinguist/include/messageitem.h \
    $$PWD/ElcoLinguist/include/languagelistdialog.h \
    $$PWD/ElcoLinguist/include/customitemdelegate.h \
    $$PWD/ElcoLinguist/include/filereader.h \
    $$PWD/ElcoLinguist/include/filewriter.h

FORMS += \
    $$PWD/ElcoLinguist/ui/aboutdialog.ui \
    $$PWD/ElcoLinguist/ui/mainwindow.ui \
    $$PWD/ElcoLinguist/ui/settingsdialog.ui \
    $$PWD/ElcoLinguist/ui/exportlanguagesdialog.ui

RESOURCES += \
    $$PWD/icons/icons.qrc \
    $$PWD/translations/translations.qrc

TRANSLATIONS += $$PWD/translations/elcolinguist_it.ts
TRANSLATIONS += $$PWD/translations/elcolinguist_en.ts

