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

private:
    std::vector<std::pair<std::string, Language>> m_languages;

    int findKeyString(const std::string &str) const;
    void updateItemsData();
};
