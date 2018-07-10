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
    const std::vector<Language> getLanguages(std::vector<std::string> languagesToExport);
    const std::vector<std::string> getLanguagesName();
    void setDefault(const Language &def);
    bool insertLanguage(const std::string &languageName, const Language &language);
    languageTableModel *getTableByContext(std::string context = "");

    void clear();

private slots:
    void updateItemData(QStandardItem *changedItem);

private:
    Language defaultLanguage;
    std::unordered_map<std::string, Language> languages;
    languageTableModel *languagesTable;
};

#endif // INCLUDELANGUAGETABLEMANAGER_H
