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
    if (exists(languageName) || languageName == "Default") {
        return;
    }

    m_languages.push_back(std::make_pair(languageName, language));
    updateItemsData();
}

void LanguagesModel::removeLanguages(const std::vector<std::string> &names)
{
    for (const auto &name : names) {
        const auto column = findKeyString(name);
        m_languages.erase(m_languages.begin() + column);
        removeColumn(column);
    }
}

bool LanguagesModel::exists(const std::string &name) const noexcept
{
    return findKeyString(name) != -1;
}

QVector<QString> LanguagesModel::languageNames() const noexcept
{
    QVector<QString> ret;
    for (const auto &item : m_languages) {
        ret.push_back(item.first.c_str());
    }

    return ret;
}

QVector<Language> LanguagesModel::languages() const noexcept
{
    QVector<Language> ret;
    for (const auto &item : m_languages) {
        ret.push_back(item.second);
    }

    return ret;
}

void LanguagesModel::replace(const std::string &oldMsg, const std::string &newMsg)
{
    for (auto &l : m_languages) {
        l.second.replaceMessage(oldMsg, newMsg);
    }
    reloadModel();
}

bool LanguagesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role)

    auto it = item(index.row(), index.column());
    auto oldText = it->text();
    auto text = value.toString();

    it->setText(text);
    setItem(index.row(), index.column(), it);
    return m_languages.at(index.column())
        .second.changeMessage(oldText.toStdString(), text.toStdString());
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

void LanguagesModel::reloadModel()
{
    clear();
    auto i = 0;
    for (const auto &language : m_languages) {
        ++i;
        decltype(auto) lastInserted = language.second;
        appendColumn(lastInserted.getMessagesByContext());
        setHorizontalHeaderItem(i - 1, new QStandardItem(lastInserted.getName().c_str()));
    }
}
