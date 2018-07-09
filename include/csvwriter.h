#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "csv.h"
#include "language.h"


class CSVwriter : public csv
{
public:
    CSVwriter();
    void setKeys(std::vector<Key> keys);
    void addLanguage(const Language &language);
private:
    int languagesCount;
    std::string infKeyText;
};

#endif // CSVWRITER_H
