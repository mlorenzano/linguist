#include "filewriter.h"

#include "qtcsv/stringdata.h"
#include "qtcsv/writer.h"

FileWriter::FileWriter(const std::string &filename,
                       const std::vector<Language> &language,
                       QObject *caller)
    : m_filename{filename}
    , m_languages{language}
    , m_caller{caller}
{}

void FileWriter::run()
{
    save();
}

void FileWriter::save()
{
    QStringList strList;
    QtCSV::StringData strData;

    strList << "Please, do not edit the \"Key\" column. It's used to identify the message.";
    strData.addRow(strList);

    strList.clear();

    strList << "Key";
    for (const auto &language : m_languages) {
        strList << QString::fromStdString(language.getName());
    }
    strData.addRow(strList);

    strList.clear();
    for (size_t i = 0; i < m_languages.front().getMessages().size(); ++i) {
        strList << QString::fromStdString(m_languages.front().getKeys().at(i).toString());
        for (auto lang : m_languages) {
            strList << lang.getMessages().at(i).c_str();
        }
        strData.addRow(strList);
        strList.clear();
    }
    QtCSV::Writer::write(QString::fromStdString(m_filename), strData, ";");
    QMetaObject::invokeMethod(m_caller, "showFinishExport", Qt::QueuedConnection);
}
