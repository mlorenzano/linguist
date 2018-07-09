#include "languagestablemanager.h"

languagesTableManager::languagesTableManager(QObject *parent) :
    QObject(parent),
    languages()
{
    languagesTable = new languageTableModel();
    connect(languagesTable, &languageTableModel::itemChanged, this, &languagesTableManager::updateItemData);
}

const std::vector<Language> languagesTableManager::getLanguages()
{
    std::vector<Language> tmp;
    tmp.push_back(defaultLanguage);
    std::transform(languages.begin(), languages.end(), std::back_inserter(tmp),
                   [] (std::pair<std::string, Language> pair)
    {
        return pair.second;
    });
    return tmp;
}

languageTableModel *languagesTableManager::getTableByContext(std::string context)
{
    languagesTable->reset();

    languagesTable->appendColumn(defaultLanguage.getMessagesByContext(context));
    languagesTable->setHorizontalHeaderItem(0, new QStandardItem("Default"));
    int i = 1;
    for (auto lang : languages) {
        languagesTable->appendColumn(lang.second.getMessagesByContext(context));
        languagesTable->setHorizontalHeaderItem(i, new QStandardItem(QString::fromStdString(lang.first)));
        ++i;
    }
    return languagesTable;
}

void languagesTableManager::setDefault(const Language &def)
{
    defaultLanguage = def;
}

bool languagesTableManager::insertLanguage(const std::string &languageName, const Language &language)
{
    if (languages.find(languageName) != languages.end())
        return false;
    languages.insert(std::make_pair(languageName, language));
    return true;
}

void languagesTableManager::updateItemData(QStandardItem *changedItem)
{
    auto changedMessageItem = (messageItem *) changedItem;
    languages.at(changedMessageItem->getLanguage()).changeMessage
            (changedMessageItem->text().toStdString(), changedMessageItem->getKey());
    changedMessageItem->changeColor();
}

void languagesTableManager::clear()
{
    languages.clear();
    languagesTable->reset();
}
