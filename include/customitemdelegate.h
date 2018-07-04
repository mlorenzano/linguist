#pragma once

#include <QTextEdit>
#include <QKeyEvent>
#include <QItemDelegate>
#include <QDebug>

class PlainTextEditor : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(QString plainText READ toPlainText WRITE setPlainText USER true)
public:
    inline PlainTextEditor(QWidget *parent) : QTextEdit(parent) {
        setAcceptRichText(false);
    }

signals:
    void editFinished();

private slots:
    void keyPressEvent(QKeyEvent *e) {
        if ((e->key() == Qt::Key_Execute || e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&
                !(e->modifiers() & Qt::ShiftModifier)) {
            e->accept();
            emit editFinished();
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

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:
    void editingFinished();

};
