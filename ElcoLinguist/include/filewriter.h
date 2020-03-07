#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "language.h"

class FileWriter
{
public:
    FileWriter(std::string filename);
    void setKeys(std::vector<Key> keys);
    void addLanguages(const std::vector<Language> &languages);
    void save();

private:
    // FIXME
    //    csv *csvWriter;
    //    xlnt::workbook *xlsxWriter;
    std::string filename;

    int languagesCount;
    std::string infKeyText;
};

#endif // CSVWRITER_H
