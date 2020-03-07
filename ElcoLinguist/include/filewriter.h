#pragma once

#include "language.h"

class FileWriter
{
public:
    void save(const std::string &filename,
              const std::vector<Language> &languages);
private:
};
