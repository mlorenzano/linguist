#pragma once

#include <QSortFilterProxyModel>

class LanguagesModel;

class TableFilter : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    void setSourceModel(QAbstractItemModel *sourceModel) override;
    void setContext(QString context);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    LanguagesModel *m_model;
    QString m_currentContext;
};
