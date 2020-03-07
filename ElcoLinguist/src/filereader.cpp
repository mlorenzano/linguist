#include "filereader.h"

#include "qtcsv/reader.h"

#include <QFile>
#include <QtDebug>

#include <algorithm>

FileReader::FileReader(std::string filename)
{
    if (filename.find("csv") != std::string::npos) {
        m_csvData = QtCSV::Reader::readToList(filename.data(), ";", "\"");
    }
}

std::vector<Key> FileReader::collectKeys()
{
    std::vector<std::string> stringKeys = collectColumnAt(0);
    std::vector<Key> keys;
    std::transform(stringKeys.begin(),
                   stringKeys.end(),
                   std::back_inserter(keys),
                   [](const std::string &key) { return Key(key); });
    return keys;
}

std::vector<std::string> FileReader::collectColumnAt(std::size_t i)
{
    std::vector<std::string> v;

    //    if (csvReader) {
    //        auto tmp = csvReader->column((int) i);
    //        if (tmp.size() > 2)
    //            std::transform(tmp.begin() + 2,
    //                           tmp.end(),
    //                           std::back_inserter(v),
    //                           [](const QString &var) { return var.toStdString(); });
    //    } else {
    //        auto ws = xlsxReader->active_sheet();
    //        auto colStart = ws.columns(false)[i].begin();
    //        std::advance(colStart, 2);
    //        std::transform(colStart,
    //                       ws.columns(false)[i].end(),
    //                       std::back_inserter(v),
    //                       [](const xlnt::cell &var) { return var.to_string(); });
    //    }
    return v;
}

std::vector<std::string> FileReader::collectIntestations()
{
    std::vector<std::string> v;
    auto intestations = m_csvData.at(1);

    for (auto item : intestations) {
        v.push_back(item.toStdString());
    }

    return v;
}
