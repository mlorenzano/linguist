#pragma once

#include "key.h"

#include <vector>
#include <QList>

class FileReader
{
public:
    explicit FileReader(std::string filename);

    std::vector<Key> collectKeys();
    std::vector<std::string> collectColumnAt(std::size_t index);
    std::vector<std::string> collectIntestations();

private:
    enum class ROW_TYPE {
        DESCRIPTION,
        INTESTATION,
        STARTIG_DATA,

        PLACEHOLDER_LAST
    };

    QList<QStringList> m_csvData;
};
