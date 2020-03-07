#pragma once

#include "key.h"

#include <vector>
#include <QList>

class FileReader
{
public:
    explicit FileReader(std::string filename);

    std::vector<Key> collectKeys();
    std::vector<std::string> collectColumnAt(std::size_t i);
    std::vector<std::string> collectIntestations();

private:
    QList<QStringList> m_csvData;
};
