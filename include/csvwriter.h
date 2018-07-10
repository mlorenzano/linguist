#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "csv.h"
#include "language.h"


class CSVwriter : public csv
{
public:
    CSVwriter();
    void setKeys(std::vector<Key> keys);
    void addLanguages(const std::vector<Language> &languages);
private:
    int languagesCount;
    std::string infKeyText;
};

#endif // CSVWRITER_H
