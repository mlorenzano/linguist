#include "customitemdelegate.h"
#include <QAbstractTableModel>
#include <QTimer>

CustomItemDelegate::CustomItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *CustomItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    PlainTextEditor *pt = new PlainTextEditor(parent);
    connect(pt, SIGNAL(editFinished()), this, SLOT(editingFinished()));
    return pt;
}

void CustomItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto textEditor = dynamic_cast<PlainTextEditor *>(editor);
    if (textEditor)
        textEditor->setPlainText(index.data(Qt::DisplayRole).toString());
}
void CustomItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto textEditor = dynamic_cast<PlainTextEditor *>(editor);
    if (textEditor)
        model->setData(index, QVariant(textEditor->toPlainText()));
}

void CustomItemDelegate::editingFinished()
{
    if (PlainTextEditor *editor = dynamic_cast<PlainTextEditor*>(sender())) {
        emit commitData(editor);
        emit closeEditor(editor);
    }
}

