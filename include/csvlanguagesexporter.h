#pragma once

#include "languagesexporter.h"

class CsvLanguagesExporterPrivate;
class QFile;
class QTextStream;

class CsvLanguagesExporter : public AbstractLanguagesExporter
{
    Q_OBJECT

public:
    explicit CsvLanguagesExporter(LanguagesManager *manager, QObject *parent = 0);
    virtual ~CsvLanguagesExporter();

    QString supportedFileType() const;

protected:
    bool openFileForExport();
    bool writeFileMetadata(const QStringList &header);
    bool addTranslation(const LanguageKey &key, const QString &source, const QStringList &translations);
    bool createExportFile();

    bool openFileForImport();
    QStringList languages() const;
    LanguageKey key() const;
    QString source() const;
    QStringList translations() const;
    bool next();

private:
    QString encodeString(QString string);

    CsvLanguagesExporterPrivate *d;
};
