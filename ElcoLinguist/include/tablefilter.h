#pragma once

#include "key.h"
#include "language.h"

#include <QSortFilterProxyModel>

class LanguagesModel;

class TableFilter : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TableFilter(const std::vector<Key> &keys);

    void setSourceModel(QAbstractItemModel *sourceModel) override;
    void setFilter(const QString &context, const QString &page);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    LanguagesModel *m_model;
    QVector<Language> m_languages;
    QString m_currentContext;
    QString m_currentPage;
    const std::vector<Key> &m_keys;
};
