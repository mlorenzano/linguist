#include "languagestablemanager.h"

languagesTableManager::languagesTableManager(QObject *parent) :
    QObject(parent),
    languages()
{
    languagesTable = new languageTableModel();
    connect(languagesTable, &languageTableModel::itemChanged, this, &languagesTableManager::updateItemData);
}


languageTableModel *languagesTableManager::getTable(const std::string &context,
                                                    const std::string &page,
                                                    const std::vector<std::string> languagesName)
{
    languagesTable->reset();
    auto a = defaultLanguage.getMessagesByContext(context, page);
    languagesTable->appendColumn(a);
    languagesTable->setHorizontalHeaderItem(0, new QStandardItem("Default"));
    int i = 1;
    for (auto lang : languagesName) {
        auto pos = findKeyString(lang);
        auto b = languages.at(pos).second.getMessagesByContext(context, page);
        languagesTable->appendColumn(b);
        languagesTable->setHorizontalHeaderItem(i, new QStandardItem(QString::fromStdString(lang)));
        ++i;
    }
    return languagesTable;
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

const std::vector<Language> languagesTableManager::getLanguages(const std::vector<std::string> &languagesToExport)
{
    std::vector<Language> tmp;
    for (auto i : languagesToExport) {
        auto pos = findKeyString(i);
        tmp.push_back(languages.at(pos).second);
    }
    return tmp;
}

const std::vector<std::string> languagesTableManager::getLanguagesName()
{
    std::vector<std::string> tmp;
    std::transform(languages.begin(), languages.end(), std::back_inserter(tmp),
                   [] (std::pair<std::string, Language> pair)
    {
        return pair.first;
    });
    return tmp;
}

void languagesTableManager::setDefault(const Language &def)
{
    defaultLanguage = def;
}

void languagesTableManager::removeLanguages(const std::vector<std::string> &languagesToRemove)
{
    for (auto i : languagesToRemove) {
        auto pos = findKeyString(i);
        languages.erase(languages.begin() + pos);
    }
}
bool languagesTableManager::insertLanguage(const std::string &languageName, const Language &language)
{
    if (/*findKeyString(languageName) < 0 || */languageName == "Default") {
        return false;
    }
    languages.push_back(std::make_pair(languageName, language));
    return true;
}

void languagesTableManager::updateItemData(QStandardItem *changedItem)
{
    auto changedMessageItem = (messageItem *) changedItem;
    auto pos = findKeyString(changedMessageItem->getLanguage());
    languages.at(pos).second.changeMessage(changedMessageItem->text().toStdString(),
                                           changedMessageItem->getKey());
    changedMessageItem->changeColor();
    emit dataChanged();
}

int languagesTableManager::findKeyString(const std::string &str)
{
    auto it = std::find_if(languages.begin(), languages.end(),
                           [&](auto &item){return item.first == str;});
    return std::distance(languages.begin(), it);
}

void languagesTableManager::clear()
{
    languages.clear();
    languagesTable->reset();
}
