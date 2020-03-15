#include "tablefilter.h"

#include "LanguagesModel.hpp"

#include <QDebug>

TableFilter::TableFilter(const std::vector<Key> &keys)
    : m_model{nullptr},
    m_keys{keys}
{}

void TableFilter::setSourceModel(QAbstractItemModel *sourceModel)
{
    m_model = dynamic_cast<LanguagesModel *>(sourceModel);
    m_languages = m_model->languages();
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

void TableFilter::setFilter(const QString &context, const QString &page)
{
    m_currentContext = context;
    m_currentPage = page;
    invalidateFilter();
}

void TableFilter::setFilteredLanguages(const QVector<uint16_t> &filteredLanguages)
{
    m_indexFilteredLang = filteredLanguages;
}

bool TableFilter::filterAcceptsRow(int source_row,
                                   const QModelIndex &source_parent) const
{
    if (m_currentContext.isEmpty() && m_currentPage.isEmpty() &&
        filterRegExp().isEmpty()) {
        return true;
    }

    bool filterOk = false;
    QString context = QString::fromStdString(m_keys.at(source_row).getContext());
    QString page = QString::fromStdString(m_keys.at(source_row).getPageOfContext());
    QString text;

    if (!m_currentContext.isEmpty()) {
        if (context == m_currentContext) {
            if (!m_currentPage.isEmpty() && page == m_currentPage) {
                filterOk = true;
            } else if (m_currentPage.isEmpty()) {
                filterOk = true;
            }
        }
    } else {
        filterOk = true;
    }

    if (filterOk && !filterRegExp().isEmpty()) {
        filterOk = false;
        for (int col = 0; col < m_languages.size(); ++col) {
            auto model = sourceModel()->index(source_row, col, source_parent);
            text = sourceModel()->data(model).toString();
            if (filterRegExp().exactMatch(text)) {
                filterOk = true;
                break;
            }
        }
    }

    return filterOk;
}

bool TableFilter::filterAcceptsColumn(int source_column,
                                      const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    return m_indexFilteredLang.contains(source_column);
}
