#ifndef INCLUDECSVREADER_H
#define INCLUDECSVREADER_H

#include "csv.h"
#include "key.h"

class CSVreader : csv
{
public:
    CSVreader();
    CSVreader(std::string filename);

    std::vector<Key> collectKeys();
    std::vector<std::string> collectColumnAt(std::size_t i);
    std::vector<std::string> collectIntestations();
};

#endif // INCLUDECSVREADER_H
