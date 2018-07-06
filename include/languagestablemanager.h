#ifndef INCLUDELANGUAGETABLEMANAGER_H
#define INCLUDELANGUAGETABLEMANAGER_H

#include "language.h"

#include <QStandardItem>

#include <unordered_map>
#include <list>

class languagesTableManager
{
public:
    languagesTableManager();
    void insertLanguage(std::string languageName, Language language);
    QStandardItemModel *getTableByContext(std::string context = "");

private:
    QStandardItemModel *languagesTable;
    std::unordered_map<std::string, Language> languages;
};

#endif // INCLUDELANGUAGETABLEMANAGER_H
