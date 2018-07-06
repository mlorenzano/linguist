#ifndef INCLUDELANGUAGETABLEMANAGER_H
#define INCLUDELANGUAGETABLEMANAGER_H

#include "language.h"
#include "languagetablemodel.h"

#include <unordered_map>
#include <list>

class languagesTableManager
{
public:
    languagesTableManager();
    void insertLanguage(std::string languageName, Language language);
    languageTableModel *getTableByContext(std::string context = "");

private:
    languageTableModel *languagesTable;
    std::unordered_map<std::string, Language> languages;
};

#endif // INCLUDELANGUAGETABLEMANAGER_H
