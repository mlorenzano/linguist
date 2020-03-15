#pragma once

#include <QTextEdit>
#include <QKeyEvent>
#include <QItemDelegate>
#include <QDebug>
#include <QApplication>

class PlainTextEditor : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(QString plainText READ toPlainText WRITE setPlainText USER true)
public:
    inline PlainTextEditor(QWidget *parent) : QTextEdit(parent) {
        setAcceptRichText(false);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

signals:
    void editFinished();

private slots:
    void keyPressEvent(QKeyEvent *e) {
        if ((e->key() == Qt::Key_Execute || e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)) {
            if ((e->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
                insertPlainText(QString("\n"));
            }
            else
                emit editFinished();
            e->accept();
        }
        else {
            QTextEdit::keyPressEvent(e);
        }
    }

};

class CustomItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CustomItemDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private slots:
    void editingFinished();

};
