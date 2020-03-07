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
        auto b = languages[lang].getMessagesByContext(context, page);
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
        tmp.push_back(languages[i]);
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
        languages.erase(i);
    }
}
bool languagesTableManager::insertLanguage(const std::string &languageName, const Language &language)
{
    if (languages.find(languageName) != languages.end() || languageName == "Default")
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
    emit dataChanged();
}

void languagesTableManager::clear()
{
    languages.clear();
    languagesTable->reset();
}
