#-------------------------------------------------
#
# Project created by QtCreator 2018-07-02T16:28:40
#
#-------------------------------------------------

QT       += core gui xml

CONFIG += c++14

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

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/libs-3rdparty/libXLNT -lxlntd
} else {
    LIBS += -L$$PWD/libs-3rdparty/libXLNT -lxlnt
}

INCLUDEPATH += $$PWD/src \
               $$PWD/include \
               $$PWD/ui \
               $$PWD/libs-3rdparty/libCSV/shared \
               $$PWD/libs-3rdparty/libXLNT/shared

SOURCES += \
    src/aboutdialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingsdialog.cpp \
    src/language.cpp \
    src/key.cpp \
    src/languagestablemanager.cpp \
    src/messageitem.cpp \
    src/csvwriter.cpp \
    src/csvreader.cpp \
    src/languagelistdialog.cpp \
    src/customitemdelegate.cpp

HEADERS += \
    libs-3rdparty/libCSV/shared/csv.h \
    libs-3rdparty/libCSV/shared/csv_global.h \
    include/aboutdialog.h \
    include/mainwindow.h \
    include/settingsdialog.h \
    include/language.h \
    include/key.h \
    include/languagestablemanager.h \
    include/languagetablemodel.h \
    include/messageitem.h \
    include/csvwriter.h \
    include/csvreader.h \
    include/languagelistdialog.h \
    libs-3rdparty/libXLNT/shared/xlnt/cell/cell.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/cell/cell_reference.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/cell/cell_type.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/cell/comment.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/cell/index_types.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/cell/rich_text.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/cell/rich_text_run.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/packaging/manifest.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/packaging/relationship.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/packaging/uri.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/styles/alignment.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/styles/border.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/styles/color.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/styles/conditional_format.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/styles/fill.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/styles/font.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/styles/format.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/styles/number_format.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/styles/protection.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/styles/style.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/utils/calendar.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/utils/date.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/utils/datetime.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/utils/exceptions.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/utils/optional.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/utils/path.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/utils/scoped_enum_hash.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/utils/time.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/utils/timedelta.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/utils/variant.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/calculation_properties.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/document_security.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/external_book.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/metadata_property.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/named_range.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/streaming_workbook_reader.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/streaming_workbook_writer.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/theme.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/workbook.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/workbook_view.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/workbook/worksheet_iterator.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/cell_iterator.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/cell_vector.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/column_properties.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/header_footer.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/major_order.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/page_margins.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/page_setup.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/pane.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/range.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/range_iterator.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/range_reference.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/row_properties.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/selection.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/sheet_protection.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/sheet_view.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/worksheet/worksheet.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/xlnt_config.hpp \
    libs-3rdparty/libXLNT/shared/xlnt/xlnt.hpp \
    include/customitemdelegate.h

FORMS += \
    ui/aboutdialog.ui \
    ui/mainwindow.ui \
    ui/settingsdialog.ui \
    ui/exportlanguagesdialog.ui

RESOURCES += \
    resources.qrc
