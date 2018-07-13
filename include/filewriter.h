#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "language.h"

#include <csv.h>
#include <xlnt/xlnt.hpp>

class FileWriter
{
public:
    FileWriter(std::string filename);
    void setKeys(std::vector<Key> keys);
    void addLanguages(const std::vector<Language> &languages);
    void save();
private:
    Csv *csvWriter;
    xlnt::workbook *xlsxWriter;
    std::string filename;

    int languagesCount;
    std::string infKeyText;
};

#endif // CSVWRITER_H
