#pragma once

#include "languagekey.h"

#include <QObject>
#include <QStringList>

class LanguagesManager;

class AbstractLanguagesExporter : public QObject
{
    Q_OBJECT

public:
    explicit AbstractLanguagesExporter(LanguagesManager *manager, QObject *parent = 0);
    virtual ~AbstractLanguagesExporter();

    int translationsCount() const;

    void setFilename(const QString &filename);
    QString filename() const;

    bool exportLanguages(QStringList filter = QStringList());
    bool importLanguages();

    virtual QString supportedFileType() const = 0;

protected:

    // Export functions --------------------------------------------------------

    /*!
     * Opens the file to begin the export.
     */
    virtual bool openFileForExport() = 0;

    /*!
     * Writes, if necessary, the metadata of the file.
     */
    virtual bool writeFileMetadata(const QStringList &header) = 0;

    /*!
     * Adds a new message to the file.
     */
    virtual bool addTranslation(const LanguageKey &key, const QString &source, const QStringList &translations) = 0;

    /*!
     * Creates the file with all the indicated messages.
     */
    virtual bool createExportFile() = 0;

    // Import functions --------------------------------------------------------

    /*!
     * Tries to open the indicated file and sets the available languages.
     */
    virtual bool openFileForImport() = 0;

    /*!
     * Languages found in the current file.
     */
    virtual QStringList languages() const = 0;

    /*!
     * Current key of the message.
     *
     * Changes every time next() is called.
     */
    virtual LanguageKey key() const = 0;

    /*!
     * Message in it's default languages.
     *
     * Changes every time next() is called.
     */
    virtual QString source() const = 0;

    /*!
     * Translations of the message.
     *
     * The translations are in the order specified by languages(). Empty
     * translations are indicated with emtpy strings. Changes every time next()
     * is called.
     */
    virtual QStringList translations() const = 0;

    /*!
     * Finds next available translation.
     *
     * When called, searches for the next available translation in the file and
     * updates the current translation status (key(), source() and
     * translations()). If no more messages are available, returns false.
     */
    virtual bool next() = 0;

    const QString KEY_COLUMN_WARNING;

private:
    QString m_filename;
    LanguagesManager *m_manager;
};
