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

    languageTableModel *getTable(const std::string &context, const std::vector<std::string> languagesName);

    const std::vector<Language> getLanguages();
    const std::vector<Language> getLanguages(const std::vector<std::string> &languagesToExport);

    const std::vector<std::string> getLanguagesName();
    void setDefault(const Language &def);

    void removeLanguages(const std::vector<std::string> &languagesToRemove);
    bool insertLanguage(const std::string &languageName, const Language &language);

    void clear();
signals:
    void dataChanged();

private slots:
    void updateItemData(QStandardItem *changedItem);

private:
    Language defaultLanguage;
    std::unordered_map<std::string, Language> languages;
    languageTableModel *languagesTable;
};

#endif // INCLUDELANGUAGETABLEMANAGER_H
