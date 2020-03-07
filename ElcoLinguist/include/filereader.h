#ifndef INCLUDECSVREADER_H
#define INCLUDECSVREADER_H

#include "key.h"

#include <csv.h>
#include <xlnt/xlnt.hpp>


class FileReader
{
public:
    FileReader();
    FileReader(std::string filename);

    std::vector<Key> collectKeys();
    std::vector<std::string> collectColumnAt(std::size_t i);
    std::vector<std::string> collectIntestations();
private:
    csv *csvReader;
    xlnt::workbook *xlsxReader;

};

#endif // INCLUDECSVREADER_H
