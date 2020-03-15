QT       += core gui xml concurrent

CONFIG += c++1z


CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS += -O3
}

CONFIG(debug, debug|release) {
unix: {
    CONFIG += sanitizer
    CONFIG += sanitize_address
    CONFIG += sanitize_undefined
}
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
    $$PWD/src/src \
    $$PWD/src/include \
    $$PWD/src/ui

SOURCES += \
    $$PWD/src/src/main.cpp \
    $$PWD/src/src/aboutdialog.cpp \
    $$PWD/src/src/mainwindow.cpp \
    $$PWD/src/src/settingsdialog.cpp \
    $$PWD/src/src/language.cpp \
    $$PWD/src/src/key.cpp \
    $$PWD/src/src/messageitem.cpp \
    $$PWD/src/src/languagelistdialog.cpp \
    $$PWD/src/src/customitemdelegate.cpp \
    $$PWD/src/src/filereader.cpp \
    $$PWD/src/src/filewriter.cpp \
    $$PWD/src/src/LanguagesModel.cpp \
    $$PWD/src/include/tablefilter.cpp \
    $$PWD/src/src/searchform.cpp

HEADERS += \
    $$PWD/src/include/aboutdialog.h \
    $$PWD/src/include/mainwindow.h \
    $$PWD/src/include/settingsdialog.h \
    $$PWD/src/include/language.h \
    $$PWD/src/include/key.h \
    $$PWD/src/include/messageitem.h \
    $$PWD/src/include/languagelistdialog.h \
    $$PWD/src/include/customitemdelegate.h \
    $$PWD/src/include/filereader.h \
    $$PWD/src/include/filewriter.h \
    $$PWD/src/include/LanguagesModel.hpp \
    $$PWD/src/include/tablefilter.h \
    $$PWD/src/include/searchform.h

FORMS += \
    $$PWD/src/ui/aboutdialog.ui \
    $$PWD/src/ui/mainwindow.ui \
    $$PWD/src/ui/settingsdialog.ui \
    $$PWD/src/ui/exportlanguagesdialog.ui \
    $$PWD/src/ui/searchform.ui

RESOURCES += \
    $$PWD/icons/icons.qrc \
    $$PWD/translations/translations.qrc

TRANSLATIONS += $$PWD/translations/src_it.ts
TRANSLATIONS += $$PWD/translations/src_en.ts

