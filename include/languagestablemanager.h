#ifndef INCLUDELANGUAGETABLEMANAGER_H
#define INCLUDELANGUAGETABLEMANAGER_H

#include "language.h"
#include "languagetablemodel.h"

#include <QObject>

#include <unordered_map>
#include <list>

class languagesTableManager : public QObject
{
    Q_OBJECT
public:
    languagesTableManager(QObject *parent = nullptr);

    const std::vector<Language> getLanguages();
    bool insertLanguage(std::string languageName, Language language);
    languageTableModel *getTableByContext(std::string context = "");

    void clear();

private slots:
    void updateItemData(QStandardItem *changedItem);

private:
    languageTableModel *languagesTable;
    std::unordered_map<std::string, Language> languages;
};

#endif // INCLUDELANGUAGETABLEMANAGER_H
