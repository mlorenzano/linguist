#include "LanguagesModel.hpp"

Qt::ItemFlags LanguagesModel::flags(const QModelIndex &index) const
{
    return (index.column() != 0) ? Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable
                                 : Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void LanguagesModel::setDefault(const Language &def)
{
    m_languages.push_back(std::make_pair("Default", def));
    updateItemsData();
}

void LanguagesModel::insertLanguage(const std::string &languageName, const Language &language)
{
    if (findKeyString(languageName) != -1 || languageName == "Default") {
        return;
    }

    m_languages.push_back(std::make_pair(languageName, language));
    updateItemsData();
}

int LanguagesModel::findKeyString(const std::string &str) const
{
    auto it = std::find_if(m_languages.cbegin(), m_languages.cend(), [&](const auto &item) {
        return item.first == str;
    });

    if (it == m_languages.cend()) {
        return -1;
    }

    return std::distance(m_languages.cbegin(), it);
}

void LanguagesModel::updateItemsData()
{
    decltype(auto) lastInserted = m_languages.back().second;
    appendColumn(lastInserted.getMessagesByContext());
    setHorizontalHeaderItem(m_languages.size() - 1,
                            new QStandardItem(lastInserted.getName().c_str()));
}
