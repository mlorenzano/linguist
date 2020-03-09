#include "tablefilter.h"
#include "LanguagesModel.hpp"

#include <QDebug>

void TableFilter::setSourceModel(QAbstractItemModel *sourceModel)
{
    m_model = dynamic_cast<LanguagesModel*>(sourceModel);
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

void TableFilter::setContext(QString context)
{
    m_currentContext = context;
}

bool TableFilter::filterAcceptsRow(int source_row,
                                   const QModelIndex &source_parent) const
{
    if (!filterRegExp().isEmpty()) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }

    return true;
}
