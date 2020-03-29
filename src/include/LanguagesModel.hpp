#pragma once

#include "language.h"

#include <QAbstractTableModel>

class LanguagesModel final : public QStandardItemModel
{
    Q_OBJECT

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setDefault(const Language &def);

    void insertLanguage(const std::string &languageName, const Language &language);
    void removeLanguages(const std::vector<std::string> &names);

    bool exists(const std::string &name) const noexcept;

    QVector<QString> languageNames() const noexcept;
    QVector<Language> languages() const noexcept;

    void replace(const std::string &oldMsg, const std::string &newMsg, int caseSensitivity);
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    std::vector<std::pair<std::string, Language>> m_languages;

    int findKeyString(const std::string &str) const;
    void updateItemsData();
    void reloadModel();
};
