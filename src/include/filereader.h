#pragma once

#include "Keys.hpp"

#include <vector>
#include <QList>

class FileReader
{
public:
    explicit FileReader(const std::string &filename);
    explicit FileReader(const QString &filename);

    Keys collectKeys();
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
