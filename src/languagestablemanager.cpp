#include "languagestablemanager.h"

languagesTableManager::languagesTableManager(QObject *parent) :
    QObject(parent),
    languages()
{}

languageTableModel *languagesTableManager::getTableByContext(std::string context)
{
    languagesTable = new languageTableModel();
    connect(languagesTable, &languageTableModel::itemChanged, this, &languagesTableManager::updateItemData);
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

void languagesTableManager::updateItemData(QStandardItem *changedItem)
{
    std::cerr<<"language: "<<((messageItem *)changedItem)->getLanguage()<<std::endl;
}
