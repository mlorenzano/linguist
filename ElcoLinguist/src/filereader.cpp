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

std::vector<std::string> FileReader::collectColumnAt(std::size_t index)
{
    std::vector<std::string> v;

    for (auto i = static_cast<int>(ROW_TYPE::STARTIG_DATA); i < m_csvData.size(); ++i) {
        const auto item = m_csvData.at(i);
        v.push_back(item.at(index).toStdString());
    }

    return v;
}

std::vector<std::string> FileReader::collectIntestations()
{
    std::vector<std::string> v;
    auto intestations = m_csvData.at(static_cast<int>(ROW_TYPE::INTESTATION));

    for (auto item : intestations) {
        v.push_back(item.toStdString());
    }

    return v;
}
