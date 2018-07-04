#pragma once

#include "languagesmanager.h"
#include "languagekey.h"

#include <QAbstractTableModel>
#include <QMap>
#include <QStringList>

class LanguagesManagerModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    LanguagesManagerModel();

    void init();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QString context() const;
    void setContext(const QString &context);

    QStringList languages() const;
    void setLanguages(const QStringList &languages);

    LanguageKey key(const QModelIndex &index) const;

    QString standardDataFromIndex(const QModelIndex &index);

private slots:
    void managerLanguageAdded(const QString &name);
    void managerLanguageRemoved(const QString &name);

private:
    QString escaped(const QString &message) const;

    LanguagesManager *m_manager;
    QMap<int, LanguageKey> m_rowmap;
    QMap<int, int> m_columnmap;
    QString m_context;
    QStringList m_languages;
};
