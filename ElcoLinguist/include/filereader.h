#pragma once

#include "key.h"

#include <vector>

class FileReader
{
public:
    explicit FileReader(std::string filename);

    std::vector<Key> collectKeys();
    std::vector<std::string> collectColumnAt(std::size_t i);
    std::vector<std::string> collectIntestations();

private:
    // FIXME
    //    csv *csvReader;
    //    xlnt::workbook *xlsxReader;
};
