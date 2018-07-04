#include <QFile>
#include <QTextStream>

#include "csvlanguagesexporter.h"

#include "csv.h"

const int kColKey = 0;
const int kColLngStandard = 1;
const int kColLng1stTransl = 2;


class CsvLanguagesExporterPrivate
{
public:
    CsvLanguagesExporterPrivate(CsvLanguagesExporter *exporter);
    ~CsvLanguagesExporterPrivate();

    bool import();
    bool next();

    CsvLanguagesExporter *q;

    csv m_csv;
    unsigned int m_currentRow {};
    QChar m_sep;

    QFile *m_file;
    QTextStream *m_txt;

    LanguageKey m_key;
    QString m_source;
    QStringList m_translations;
    QStringList m_languages;

private:
    bool tryWithSeparator(char sep);
};


CsvLanguagesExporterPrivate::CsvLanguagesExporterPrivate(CsvLanguagesExporter *exporter) :
    q{exporter},
    m_sep{';'},
    m_file{new QFile},
    m_txt{new QTextStream{m_file}}
{
    m_txt->setCodec("UTF-8");
}

CsvLanguagesExporterPrivate::~CsvLanguagesExporterPrivate()
{
    delete m_txt;
    delete m_file;
}

bool CsvLanguagesExporterPrivate::import()
{
    return (tryWithSeparator(';') ||
            tryWithSeparator(',') ||
            tryWithSeparator(':') ||
            tryWithSeparator(0x09));
}

bool CsvLanguagesExporterPrivate::next()
{
    m_key = LanguageKey();
    m_source.clear();
    m_translations.clear();

    if (++m_currentRow >= m_csv.rowCount())
        return false;

    m_key.fromString(m_csv.item(m_currentRow, kColKey));
    m_source = m_csv.item(m_currentRow, kColLngStandard);
    for (unsigned col = kColLng1stTransl; col < m_csv.columnCount(); col++)
        m_translations << m_csv.item(m_currentRow, col);

    return true;
}

bool CsvLanguagesExporterPrivate::tryWithSeparator(char sep)
{
    m_csv.clear();
    m_csv.setSeparatore(sep);

    if (!m_csv.load(q->filename()))
        return false;

    for (m_currentRow = 0; m_currentRow < m_csv.rowCount(); m_currentRow++) {
        auto keyCell = m_csv.item(m_currentRow, kColKey);
        if (keyCell != "Key")
            continue;

        for (unsigned col = kColLng1stTransl; col < m_csv.columnCount(); col++)
            m_languages << m_csv.item(m_currentRow, col);

        break;
    }

    return (m_languages.size() > 0);
}


CsvLanguagesExporter::CsvLanguagesExporter(LanguagesManager *manager, QObject *parent) :
    AbstractLanguagesExporter(manager, parent),
    d(new CsvLanguagesExporterPrivate(this))
{
}

CsvLanguagesExporter::~CsvLanguagesExporter()
{
    delete d;
}

QString CsvLanguagesExporter::supportedFileType() const
{
    return "csv";
}

bool CsvLanguagesExporter::openFileForExport()
{
    d->m_file->setFileName(filename());
    return d->m_file->open(QIODevice::WriteOnly);
}

bool CsvLanguagesExporter::writeFileMetadata(const QStringList &header)
{
    *d->m_txt << KEY_COLUMN_WARNING << "\r\n";
    *d->m_txt << header.join(d->m_sep) << "\r\n";
    return true;
}

bool CsvLanguagesExporter::addTranslation(const LanguageKey &key,
                                          const QString &source,
                                          const QStringList &translations)
{
    QString trs;
    for (int i = 0; i < translations.size(); i++) {
        trs.append(encodeString(translations.at(i)));
        if (i + 1 < translations.size())
            trs.append(d->m_sep);
    }

    *d->m_txt << encodeString(key.toString()) << d->m_sep
              << encodeString(source) << d->m_sep
              << trs << "\r\n";
    return true;
}

bool CsvLanguagesExporter::createExportFile()
{
    d->m_file->close();
    return true;
}

bool CsvLanguagesExporter::openFileForImport()
{
    return d->import();
}

QStringList CsvLanguagesExporter::languages() const
{
    return d->m_languages;
}

LanguageKey CsvLanguagesExporter::key() const
{
    return d->m_key;
}

QString CsvLanguagesExporter::source() const
{
    return d->m_source;
}

QStringList CsvLanguagesExporter::translations() const
{
    return d->m_translations;
}

bool CsvLanguagesExporter::next()
{
    return d->next();
}

QString CsvLanguagesExporter::encodeString(QString string)
{
    QString ret = string;
    if (ret.contains(QChar('"'))) {
        QString tmp;
        for (int i = 0; i < ret.length(); i++) {
            if (ret.at(i) == QChar('"')) {
                tmp.append(ret.at(i));
            }
            tmp.append(ret.at(i));
        }
        ret = tmp;
    }

    if (ret.contains(d->m_sep) || ret.contains(QChar('\r')) || ret.contains(QChar('\n')) ||
       ret.startsWith(QChar(' ')) || ret.endsWith(QChar(' ')) || ret.contains(QChar('"'))) {
        ret.prepend(QChar('"'));
        ret.append(QChar('"'));
    }

    return ret;
}
