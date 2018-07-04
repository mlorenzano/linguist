#include "customitemdelegate.h"
#include <QAbstractTableModel>
#include <QTimer>
#include "languagesmanagermodel.h"

CustomItemDelegate::CustomItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void CustomItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
}

QWidget *CustomItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const LanguagesManagerModel *model = dynamic_cast<const LanguagesManagerModel*>(index.model());
    if (model &&
            model->key(index).context() == eventsHandlerContext) {
        PlainTextEditor *pt = new PlainTextEditor(parent);
        connect(pt, SIGNAL(editFinished()), this, SLOT(editingFinished()));
        return pt;
    }
    else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void CustomItemDelegate::editingFinished()
{
    if (PlainTextEditor *editor = dynamic_cast<PlainTextEditor*>(sender())) {
        emit commitData(editor);
        emit closeEditor(editor);
    }
}

