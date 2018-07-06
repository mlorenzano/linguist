#include "languagestablemanager.h"

languagesTableManager::languagesTableManager() :
    languages()
{}

QStandardItemModel *languagesTableManager::getTableByContext(std::string context)
{
    languagesTable = new QStandardItemModel(0, 0);
    int i = 0;
    for (auto lang : languages) {

        languagesTable->appendColumn(lang.second.getMessagesByContext(context));
        languagesTable->setHorizontalHeaderItem(i, new QStandardItem(QString::fromStdString(lang.first)));
        ++i;
    }
    return languagesTable;
}

void languagesTableManager::insertLanguage(std::string languageName, Language language)
{
    languages.insert(std::make_pair(languageName, language));
}

