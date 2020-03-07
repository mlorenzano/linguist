#include "filewriter.h"

#include "qtcsv/writer.h"
#include "qtcsv/stringdata.h"

void FileWriter::save(const std::string &filename,
                      const std::vector<Language> &languages)
{
    QStringList strList;
    QtCSV::StringData strData;

    strList << "Please, do not edit the \"Key\" column. It's used to identify the message.";
    strData.addRow(strList);

    strList.clear();

    strList << "Key";
    for (auto language : languages) {
        strList << QString::fromStdString(language.getName());
    }
    strData.addRow(strList);

    strList.clear();
    for (auto i = 0; i < languages.front().getMessages().size(); ++i) {
        strList << QString::fromStdString(languages.front().getKeys().at(i).toString());
        for (auto lang : languages) {
            strList << lang.getMessages().at(i).c_str();
        }
        strData.addRow(strList);
        strList.clear();
    }
    QtCSV::Writer::write(QString::fromStdString(filename), strData, ";");
}
