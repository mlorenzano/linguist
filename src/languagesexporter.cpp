#include "languagesexporter.h"

#include "languagesmanager.h"
#include "languagekey.h"

#include <QSet>

AbstractLanguagesExporter::AbstractLanguagesExporter(LanguagesManager *manager, QObject *parent) :
    QObject(parent),
    KEY_COLUMN_WARNING(tr("Please, do not edit the \"Key\" column. It's used to identify the message.")),
    m_manager(manager)
{
}

AbstractLanguagesExporter::~AbstractLanguagesExporter()
{
}

int AbstractLanguagesExporter::translationsCount() const
{
    return m_manager->translatables().count();
}

void AbstractLanguagesExporter::setFilename(const QString &filename)
{
    m_filename = filename;
}

QString AbstractLanguagesExporter::filename() const
{
    return m_filename;
}

bool AbstractLanguagesExporter::exportLanguages(QStringList filter)
{
    if (!openFileForExport())
        return false;

    QStringList translations{};
    translations << QString("Key") << QString("Default");

    if (!filter.isEmpty()) {
        for (const auto &lngName : filter)
            if (lngName != QString("Default"))
                translations << lngName;
    } else {
        for (const auto &lng : m_manager->translatables())
            translations << lng->name();
    }

    if (!writeFileMetadata(translations))
        return false;

    auto lngs = m_manager->translatables();
    if (!filter.isEmpty()) {
        // It's necessary to check the languages that match with the current filter
        int i = lngs.size() - 1;
        while (i >= 0) {
            if (!filter.contains(lngs.at(i)->name()))
                lngs.removeAt(i);
            i--;
        }
    }

    for (const auto &key : m_manager->standard()->keys()) {
        translations.clear();
        const auto source = m_manager->standard()->messageTranslatable(key);
        if (source.isEmpty())
            continue;

        for (const auto &lng : lngs)
            translations << lng->translation(key);

        if (!addTranslation(key, source, translations))
            return false;
    }

    return createExportFile();
}

/*!
 * Generic algorithm for importing translated messages from the indicated file.
 *
 * Subclasses must implement the importing function for this algorithm to work.
 */
bool AbstractLanguagesExporter::importLanguages()
{
    if (!openFileForImport())
        return false;

    QList<TranslatableLanguage *> transLangs;

    foreach (const QString &langName, languages()) {
        bool found = false;
        foreach (TranslatableLanguage *language, m_manager->translatables()) {
            if (language->name() == langName) {
                transLangs << language;
                found = true;
                break;
            }
        }

        if (!found) {
            m_manager->addTranslatable(langName);
            transLangs << m_manager->translatables().last();
        }
    }

    // No sense to perform any operation, but there is no error.
    if (transLangs.isEmpty())
        return true;

    // A set provides a fast way to check if a key exists.
    QSet<LanguageKey> allKeys =
            QSet<LanguageKey>::fromList(m_manager->standard()->keys());

    while (next())
        // Cycle over all available languages.
        for (int i = 0; i < transLangs.count(); i++) {

            // Message is substitued if and only if it's key is present in the
            // Language manager.
            if (allKeys.contains(key())) {
                // Avoid message multi line for not handled data
                QString message = translations().at(i);
                if (message.contains(QChar('\n')) &&
                    !LanguagesManager::handlesNewLines(key()))
                    message.remove(QChar('\n'));
                transLangs.at(i)->setTranslation(key(), message);
            }
        }

    return true;
}
