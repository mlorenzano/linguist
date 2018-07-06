#ifndef INCLUDELANGUAGETABLEMODEL_H
#define INCLUDELANGUAGETABLEMODEL_H

#include <QStandardItemModel>

class languageTableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    //only used to make the first column non editable
    Qt::ItemFlags flags ( const QModelIndex & index ) const
    {
        if (index.column() != 0)
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
        else
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
};
#endif // INCLUDELANGUAGETABLEMODEL_H
