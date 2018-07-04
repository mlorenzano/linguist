#include "languagesmanager.h"
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

const QString languagefile_version              = "0.1";

const QString languagefile_ext                  = "k.lng";
const QString languagesmanager_folder           = "languages";
const QString languagesmanager_filename         = "languages.ini";
const QString languagesmanager_handler          = "#LNG";
const QString languagesmanager_separator        = "=";

const QString exportString_FixedMsg             = "1";
const QString exportString_DynamicMsg           = "2";
const QString exportString_SystemMsg            = "3";
const QString exportString_LanguageMsg          = "4";
const QString exportString_EventsMsg            = "5";

LanguagesManager *LanguagesManager::theInstance = 0;

LanguagesManager::LanguagesManager(QObject *parent) :
    QObject(parent),
    m_standard(new StandardLanguage),
    m_translatables(QList<TranslatableLanguage*>()),
    m_current_language(0),
    m_workingPath(QString()),
    m_dirty(false),
    m_graphic_objs(QHash<LanguageKey, int>()),
    m_modules(QList<AbstractLanguageModule*>())
{
    m_standard->setName(QString("Default"));
    m_standard->setManager(this);
/*
    SymbolTable::Table *p_symboltable = SymbolTable::Table::instance();
    connect(p_symboltable, SIGNAL(symbolRemoved(int)), this, SLOT(symbolRemoved(int)));
    connect(p_symboltable, SIGNAL(symbolChanged(int,int)), this, SLOT(symbolChanged(int,int)));
    */
}

LanguagesManager::~LanguagesManager()
{
    qDeleteAll(m_translatables);
}

LanguagesManager *LanguagesManager::instance()
{
    if (theInstance == 0)
        theInstance = new LanguagesManager;
    return theInstance;
}

StandardLanguage* LanguagesManager::standard()
{
    return m_standard;
}

QList<TranslatableLanguage*> LanguagesManager::translatables()
{
    return m_translatables;
}

QList<AbstractLadderLanguage*> LanguagesManager::languages()
{
    QList<AbstractLadderLanguage*> lngs;
    lngs << (AbstractLadderLanguage*)(m_standard);
    for (const auto &tr : m_translatables)
        lngs << (AbstractLadderLanguage*)(tr);

    return lngs;
}

bool LanguagesManager::addTranslatable(const QString &name)
{
    if (languageExists(name))
        return false;

    TranslatableLanguage *tr = new TranslatableLanguage;
    tr->setName(name);
    tr->setManager(this);
    m_translatables << tr;
    emit languageAdded(name);
    setModified(true);
    /*
    // Constant symbol handling
    int id = m_translatables.indexOf(tr) + 1;
    SymbolTable::Symbol *symbol = SymbolTable::Table::instance()->getSymbol(tr->m_symbol_id);
    if (!symbol) {
        QString address = SymbolTable::Table::instance()->firstFreeElcoAddress(SymbolTable::Symbol::Costante);
        if (!SymbolTable::Table::instance()->addNewSymbol(QString("[LNG]%1").arg(name), QString(), address, QString::number(id))) {
            qDebug() << "Error in creation symbol for language" << (name);
            return false;
        }
        symbol = SymbolTable::Table::instance()->getSymbol(address);
        tr->m_symbol_id = symbol->uniqueId();
    }

    symbol->setReadOnly(true);
    if (symbol->value().toInt() != id)
        symbol->setValue(QString::number(id));
    */
    return true;
}

bool LanguagesManager::addTranslatable(const QString &name, int symbol_id)
{
    if (languageExists(name))
        return false;

    TranslatableLanguage *tr = new TranslatableLanguage;
    tr->setName(name);
    tr->setManager(this);
    tr->m_symbol_id = symbol_id;
    m_translatables << tr;
    emit languageAdded(name);
    setModified(true);
    /*
    // Constant symbol handling
    int id = m_translatables.indexOf(tr) + 1;
    SymbolTable::Symbol *symbol = SymbolTable::Table::instance()->getSymbol(tr->m_symbol_id);
    if (!symbol) {
        QString address = SymbolTable::Table::instance()->firstFreeElcoAddress(SymbolTable::Symbol::Costante);
        if (!SymbolTable::Table::instance()->addNewSymbol(QString("[LNG]%1").arg(name), QString(), address, QString::number(id))) {
            qDebug() << "Error in creation symbol for language" << (name);
            return false;
        }
        symbol = SymbolTable::Table::instance()->getSymbol(address);
        tr->m_symbol_id = symbol->uniqueId();
    }

    symbol->setReadOnly(true);
    if (symbol->value().toInt() != id)
        symbol->setValue(QString::number(id));
    */
    return true;
}

void LanguagesManager::removeTranslatable(const QString &name)
{
    // Search the requested language
    for (int i = 0; i < m_translatables.size(); i++) {
        TranslatableLanguage *tr = m_translatables.at(i);

        if (tr->name() == name) {/*
            SymbolTable::Symbol *symbol = SymbolTable::Table::instance()->getSymbol(tr->m_symbol_id);
            if (symbol)
                SymbolTable::Table::instance()->removeSymbol(tr->m_symbol_id);
        */
            m_translatables.removeAt(i);
            delete tr;
            setModified(true);

            emit languageRemoved(name);
            break;
        }
    }

    adjustTranslatableSymbol();
}

void LanguagesManager::adjustTranslatableSymbol()
{
    /*
    for (int i = 0; i < m_translatables.size(); i++) {
        TranslatableLanguage *tr = m_translatables.at(i);
        int id = i + 1;
        SymbolTable::Symbol *symbol = SymbolTable::Table::instance()->getSymbol(tr->m_symbol_id);
        if (symbol && symbol->value().toInt() != id)
            symbol->setValue(QString::number(id));
    }*/
}

AbstractLadderLanguage* LanguagesManager::currentLanguage()
{
    QList<AbstractLadderLanguage*> lngs = languages();
    if (m_current_language < lngs.size())
        return lngs.at(m_current_language);

    return 0;
}

bool LanguagesManager::setCurrentLanguage(AbstractLadderLanguage* language)
{
    QList<AbstractLadderLanguage*> lngs = languages();
    if (lngs.contains(language)) {
        m_current_language = lngs.indexOf(language);
        return true;
    }

    return false;
}

bool LanguagesManager::setCurrentLanguage(const int index)
{
    QList<AbstractLadderLanguage*> lngs = languages();
    if (index < lngs.size()) {
        m_current_language = index;
        return true;
    }

    return false;
}

QString LanguagesManager::absoluteManagerPath() const
{
    return m_workingPath + "/" + languagesmanager_folder;
}

void LanguagesManager::setMessage(const LanguageKey &key, const QString &message)
{
    QList<AbstractLadderLanguage*> lngs = languages();
    foreach (AbstractLadderLanguage *l, lngs) {
        l->setMessage(key, message);
    }
}

void LanguagesManager::setMessage(const QString &context, const QString &id, const QString &message)
{
    QList<AbstractLadderLanguage*> lngs = languages();
    foreach (AbstractLadderLanguage *l, lngs) {
        l->setMessage(context, id, message);
    }
}

void LanguagesManager::setMessage(const QString &context, const QString &id, const QString &optional, const QString &message)
{
    QList<AbstractLadderLanguage*> lngs = languages();
    foreach (AbstractLadderLanguage *l, lngs) {
        l->setMessage(context, id, optional, message);
    }
}

int LanguagesManager::messagesCount()
{
    return m_standard->size();
}

bool LanguagesManager::languageExists(const QString &name)
{
    QList<AbstractLadderLanguage*> lngs = languages();
    foreach (AbstractLadderLanguage *l, lngs) {
        if (l->name() == name)
            return true;
    }

    return false;
}

void LanguagesManager::clearGraphicObjects()
{
    m_graphic_objs.clear();
}

void LanguagesManager::setGraphicObjects(QHash<LanguageKey, int> graphic_objs)
{
    m_graphic_objs = graphic_objs;
}

void LanguagesManager::setGraphicObject(const LanguageKey &key, int id_graphic_obj)
{
    m_graphic_objs.insert(key, id_graphic_obj);
}

void LanguagesManager::setGraphicObject(const QString &context, const QString &id, int id_graphic_obj)
{
    LanguageKey key(context, id);
    setGraphicObject(key, id_graphic_obj);
}

void LanguagesManager::setGraphicObject(const QString &context, const QString &id, const QString &optional, int id_graphic_obj)
{
    LanguageKey key(context, id, optional);
    setGraphicObject(key, id_graphic_obj);
}

bool LanguagesManager::registerModule(AbstractLanguageModule *mdl)
{
    if (m_modules.contains(mdl)) {
        qDebug() << "LanguageManager: Module already registered!";
        return false;
    }

    m_modules << mdl;

    return true;
}

void LanguagesManager::initializeModulesMessages()
{
    // Clear messages and make a copy of all translatables messages
    QList<AbstractLadderLanguage*> lngs = languages();
    QList<TranslatableLanguage*> c_translatables;
    foreach (AbstractLadderLanguage *lng, lngs) {
        if (lng->type() == TranslatableLanguage::TranslatableLanguageType) {
            TranslatableLanguage *cpy = new TranslatableLanguage;
            TranslatableLanguage *tr = (TranslatableLanguage*)(lng);
            (*cpy) = (*tr);
            c_translatables << cpy;
        }
        lng->clear();
    }

    // For each module registered, calls the inizialization off all messages
    foreach (AbstractLanguageModule *mdl, m_modules) {
        mdl->initializeMessages();
    }

    // Set translations
    for (int i = 0; i < m_translatables.size(); i++) {
        TranslatableLanguage *lng = m_translatables.at(i);
        QList<LanguageKey> keys = lng->keys();
        foreach (LanguageKey key, keys) {
            TranslatableMessage msg(c_translatables.at(i)->message(key), c_translatables.at(i)->translation(key));
            lng->setTranslation(key, msg);
        }

        delete c_translatables.at(i);
    }

    setModified(true);
}

LanguagesManager::ExportErrors LanguagesManager::exportLanguages(const QString &path)
{
    // Event exportRequested()
    emit exportRequested();

    ExportErrors ret = checkExport();
    if (ret != NoErrors)
        return ret;

    QDir export_path(path);
    if (!export_path.exists()) {
        return ExportPathNotFound;
    }

    m_export_warnings.clear();
    bool yesToAll = false;
    bool noToAll = false;
    QList<AbstractLadderLanguage*> lngs = languages();
    for (int i = 0; i < lngs.size(); i++) {
        AbstractLadderLanguage *lng = lngs[i];
        QString filename = lng->name() + languagefile_ext;
        QFile file(export_path.path() + "/" + filename);
        if (file.exists()) {
            if (!yesToAll && !noToAll) {
                QMessageBox msg;
                msg.setText(QString("%1 %2?").arg(tr("Do you want to overwrite the file")).arg(filename));
                msg.setIcon(QMessageBox::Question);
                msg.setStandardButtons(QMessageBox::Yes |
                                       QMessageBox::No |
                                       QMessageBox::YesToAll |
                                       QMessageBox::NoToAll);
                msg.setDefaultButton(QMessageBox::Yes);

                int ret = msg.exec();
                if (ret == QMessageBox::Yes ||
                        ret == QMessageBox::YesToAll) {
                    // Remove the file
                    file.remove();

                    if (ret == QMessageBox::YesToAll)
                        yesToAll = true;
                }
                else if (ret == QMessageBox::No ||
                        ret == QMessageBox::NoToAll) {
                    if (ret == QMessageBox::NoToAll)
                        noToAll = true;

                    // Continue
                    continue;
                }
            }
            else if (yesToAll) {
                // Remove the file
                file.remove();
            }
            else {
                // Continue
                continue;
            }
        }

        ret = fileExport(&file, filename, lng);
        if (ret != NoErrors)
            break;
    }

    return ret;
}

LanguagesManager::ExportErrors LanguagesManager::exportLanguage(const QString &path, const QString &language)
{
    // Event exportRequested()
    emit exportRequested();

    ExportErrors ret = checkExport();
    if (ret != NoErrors)
        return ret;

    QDir export_path(path);
    if (!export_path.exists()) {
        return ExportPathNotFound;
    }

    m_export_warnings.clear();
    QList<AbstractLadderLanguage*> lngs = languages();
    AbstractLadderLanguage *lng = 0;

    for (int i = 0; i < lngs.size(); i++) {
        if (lngs[i]->name() == language) {
            lng = lngs[i];
            break;
        }
    }

    if (!lng) {
        return LanguageNotFound;
    }

    QString filename = lng->name() + languagefile_ext;
    QFile file(export_path.path() + "/" + filename);
    if (file.exists()) {
        QMessageBox msg;
        msg.setText(QString("%1 %2?").arg(tr("Do you want to overwrite the file")).arg(filename));
        msg.setIcon(QMessageBox::Question);
        msg.setStandardButtons(QMessageBox::Yes |
                               QMessageBox::No);
        msg.setDefaultButton(QMessageBox::Yes);

        int ret = msg.exec();
        if (ret == QMessageBox::Yes) {
            // Remove the file
            file.remove();
        }
        else {
            // Return
            return NoErrors;
        }
    }

    ret = fileExport(&file, filename, lng);

    return ret;
}

// Classe per la gestione delle traduzioni delle stringhe dinamiche
class _dynamic_string_reference {
public:
    _dynamic_string_reference(QString msg, int length) {
        m_msg = msg;
        m_length = length;
    }

    QString m_msg;    // Messaggio tradotto
    int m_length;     // Lunghezza del messaggio originale
};

LanguagesManager::ExportErrors LanguagesManager::fileExport(QFile *file,
                        const QString &filename, AbstractLadderLanguage *lng)
{
    if (!file->open(QFile::ReadWrite | QIODevice::Text)) {
        qDebug() << "LanguagesManager: Error while opening file" << filename;
        return NoErrors;
    }

    QTextStream stream(file);
    stream.setCodec("UTF-8");
    stream.setGenerateByteOrderMark(true);

    ExportErrors ret ;//= streamExport(stream, lng);
    file->close();
    return ret;
}
/*
LanguagesManager::ExportErrors
    LanguagesManager::streamExport(QTextStream &stream,
                                   AbstractLadderLanguage *lng)
{
    ExportErrors ret = NoErrors;

    // Language information
    stream << QString("%1 %2")
              .arg(exportString_LanguageMsg)
              .arg(lng->name())
           << endl;

    QMap<int, _dynamic_string_reference> dynamic_msgs;

    // Messages
    for (auto it = m_graphic_objs.constBegin();
         it != m_graphic_objs.constEnd(); ++it) {
        QString str_graphic_obj = QString("%1").arg(it.value(), 6, 10, QChar('0'));
        QString str_message;

        if (lng->type() == StandardLanguage::StandardLanguageType) {
            // Standard
            str_message = lng->message(it.key());
        }
        else if (lng->type() == TranslatableLanguage::TranslatableLanguageType) {
            // Translatable
            TranslatableLanguage *tr = (TranslatableLanguage*)lng;
            str_message = tr->translation(it.key());
            if (str_message.isEmpty() ||
                    !m_standard->translatable(it.key())) {
                // Take standard message
                str_message = m_standard->message(it.key());
            }
        }

        if (str_message.isEmpty())
            continue;

        if (str_message.contains('\n') &&
            FirmwareManager::instance()->currentVersion()
            .hasFeature(FirmwareVersion::NewLinesInTranslatableStrings)) {
            if (handlesNewLines(it.key()))
                str_message.replace('\n', "\\n");
        }

        if (it.key().context() == dynamicStringsContext) {
            // Check message validation
            QString original = m_standard->message(it.key());
            if (str_message.length() != original.length() &&
                !FirmwareManager::instance()->currentVersion()
                .hasFeature(FirmwareVersion::LanguagesExport)) {
                QString old_str = str_message;
                if (str_message.length() < original.length())
                    str_message = QString("%1").arg(str_message, -original.length());
                else
                    str_message.truncate(original.length());

                m_export_warnings << QString("\"%1\" --> \"%2\" [%3]").arg(old_str).arg(str_message).arg(lng->name());
                ret = Warnings;
            }

            dynamic_msgs.insert(it.value(), _dynamic_string_reference(str_message, original.length()));
        }
        else if (it.key().context() == eventsHandlerContext &&
                 FirmwareManager::instance()->currentVersion()
                 .hasFeature(FirmwareVersion::EventsHandler)) {
            stream << QString("%1 %2 %3")
                      .arg(exportString_EventsMsg)
                      .arg(str_graphic_obj)
                      .arg(str_message)
                   << endl;
        }
        else {
            stream << QString("%1 %2 %3")
                      .arg(exportString_FixedMsg)
                      .arg(str_graphic_obj)
                      .arg(str_message)
                   << endl;
        }
    }

    // Add dynamic
    int counter = 0;
    for (auto c = dynamic_msgs.begin(); c != dynamic_msgs.end(); ++c) {
        QString msg = c.value().m_msg;
        stream << QString("%1 %2 %3")
                  .arg(exportString_DynamicMsg)
                  .arg(counter)
                  .arg(msg)
               << endl;
        counter += (c.value().m_length + 1);
        if (counter % 2 != 0)
            counter++;
    }

    return ret;
}
*/

QStringList LanguagesManager::exportWarnings() const
{
    return m_export_warnings;
}

LanguagesManager::ExportErrors LanguagesManager::checkExport()
{
    if (m_graphic_objs.isEmpty()) {
        return EmptyGraphicObjs;
    }

    return NoErrors;
}

void LanguagesManager::setRebuild()
{
    clearGraphicObjects();
}

void LanguagesManager::addLngFilesForLpuOrDownload(const QString &path)
{
    for (const auto &lng : languages()) {
        QString filename{path + "/" + lng->name() + ".lng"};
        QFile *file{new QFile(filename)};
        fileExport(file, filename, lng);
    }
}

void LanguagesManager::symbolRemoved(int id)
{
    for (const auto &tr : m_translatables) {
        if (tr->m_symbol_id == id) {
            tr->m_symbol_id = -1;
            setModified(true);
        }
    }
}

void LanguagesManager::symbolChanged(int oldId, int newId)
{
    for (const auto &tr : m_translatables) {
        if (tr->m_symbol_id == oldId) {
            tr->m_symbol_id = newId;
            setModified(true);
        }
    }
}

bool LanguagesManager::saveToBetaVersion()
{
    // Save the configuration file
    QDir path(m_workingPath);
    if (!path.exists())
        return false;

    path.setPath(absoluteManagerPath());
    if (!path.exists())
        return false;

    QFile file(path.absoluteFilePath(languagesmanager_filename));
    if (file.exists())
        file.remove();

    if (!file.open(QFile::ReadWrite))
        return false;

    QTextStream stream(&file);
    file.seek(0);

    for (const auto &tr : m_translatables) {
        stream << tr->name() << languagesmanager_separator << tr->m_symbol_id << endl;
    }

    file.close();

    return true;
}

bool LanguagesManager::loadFromBetaVersion()
{
    // Load the configuration file
    QDir path(m_workingPath);
    if (!path.exists())
        return false;

    path.setPath(absoluteManagerPath());
    if (!path.exists())
        return false;

    QFile file(path.absoluteFilePath(languagesmanager_filename));
    if (!file.exists())
        return false;

    if (!file.open(QFile::ReadOnly))
        return false;

    QTextStream stream(&file);
    file.seek(0);

    clear();
    while (!stream.atEnd()) {
        QString value = stream.readLine();
        QStringList fields = value.split(languagesmanager_separator);
        if (fields.size() > 0)
            addTranslatable(fields.at(0), (fields.size() > 1 ? fields.at(1).toInt() : -1));
    }

    file.close();

    saveCurrent();

    return true;
}

bool LanguagesManager::saveCurrent()
{
    // Save the configuration file
    QDir path(m_workingPath);
    if (!path.exists())
        return false;

    path.setPath(absoluteManagerPath());
    if (!path.exists())
        return false;

    QFile file(path.absoluteFilePath(languagesmanager_filename));
    if (file.exists())
        file.remove();

    if (!file.open(QFile::ReadWrite))
        return false;

    QTextStream stream(&file);
    file.seek(0);

    //stream << languagefile_version_string << languagefile_version << endl;

    for (const auto &tr : m_translatables) {
        stream << languagesmanager_handler << languagesmanager_separator <<  tr->name() << languagesmanager_separator << tr->m_symbol_id << endl;
    }

    file.close();

    return true;
}

bool LanguagesManager::loadCurrent()
{
    // Load the configuration file
    QDir path(m_workingPath);
    if (!path.exists())
        return false;

    path.setPath(absoluteManagerPath());
    if (!path.exists())
        return false;

    QFile file(path.absoluteFilePath(languagesmanager_filename));
    if (!file.exists())
        return false;

    if (!file.open(QFile::ReadOnly))
        return false;

    QTextStream stream(&file);
    file.seek(0);

    clear();
    while (!stream.atEnd()) {
        QString value = stream.readLine();
        QStringList fields = value.split(languagesmanager_separator);
        if (fields.size() > 1 &&
             fields.at(0) == languagesmanager_handler) {
            fields.removeFirst();
            addTranslatable(fields.at(0), (fields.size() > 1 ? fields.at(1).toInt() : -1));
        }
        else {
            // Linea non valida
        }
    }

    file.close();

    return true;
}
/*
bool LanguagesManager::setModifiedFromLastBuild(const bool b)
{
    if (CompilableModuleInterface::setModifiedFromLastBuild(b) && b)
        emit changedFromLastBuild();
    return true;
}
*/

void LanguagesManager::clear()
{
    m_standard->clear();
    for (const auto &tr : m_translatables) {
        delete tr;
    }
    m_translatables.clear();
    m_current_language = 0;
    clearGraphicObjects();
    setModified(false);
}

bool LanguagesManager::save()
{
    if (saveConfigurationOnFile()) {
        setModified(false);
        return true;
    }

    return false;
}

bool LanguagesManager::saveConfigurationOnFile()
{
    bool ret = saveCurrent();
    if (ret) {
        // Save all the languages
        QList<AbstractLadderLanguage*> lngs = languages();
        for (const auto &l : lngs) {
            l->save();
        }
    }

    return ret;
}

bool LanguagesManager::load()
{
//    bool ret = false;
//    QString ver = checkFileVersion();

//    if (ver == languagefile_version) {
//        ret = loadCurrent();
//    }
//    else if (ver == "beta") {
//        ret = loadFromBetaVersion();
//    }
//    else {
//        // Errore - nessuna corrispondenza trovata
//    }

//    if (ret) {
//        // Constant standard langauge symbol handling
//       /*SymbolTable::Symbol *symbol = SymbolTable::Table::instance()->getSymbolByName(QString("[LNG]%1").arg(m_standard->name()));
//        if (!symbol) {
//            QString address = SymbolTable::Table::instance()->firstFreeElcoAddress(SymbolTable::Symbol::Costante);
//            if (!SymbolTable::Table::instance()->addNewSymbol(QString("[LNG]%1").arg(m_standard->name()), QString(), address, QString("0")))
//                qDebug() << "Error in creation symbol for language" << (m_standard->name());

//            symbol = SymbolTable::Table::instance()->getSymbol(address);
//        }

//        symbol->setReadOnly(true);
//        if (symbol->value().toInt() != 0)
//            symbol->setValue(QString("0"));
//        */
//        // Load all the languages
//        QList<AbstractLadderLanguage*> lngs = languages();
//        for (const auto &l : lngs) {
//            l->load();
//        }

//        emit loaded();
//    }

//    setModified(false);

//    return ret;
return false;
}

QString LanguagesManager::errorDescription() const
{
    // TODO: Handle error description
    return QString();
}


void LanguagesManager::setWorkingPath(const QString &path)
{
    if (m_workingPath != path) {
        m_workingPath = path;

        QDir wP(m_workingPath + "/" + languagesmanager_folder);
        if (!wP.exists())
            wP.mkpath(wP.absolutePath());

        load();
    }
}

void LanguagesManager::setWorkingTempPath(const QString &path)
{
    m_workingTempPath = path;
}

bool LanguagesManager::modified() const
{
    return m_dirty;
}

void LanguagesManager::setModified(const bool b)
{
    if (m_dirty != b) {
        m_dirty = b;
        if (m_dirty) {
            emit changed();
        }
    }
    //setModifiedFromLastBuild(b);
}

void LanguagesManager::connectEditedSlot(QObject *receiver, const char *slot)
{
    connect(this, SIGNAL(changedFromLastBuild()), receiver, slot);
}

bool LanguagesManager::handlesNewLines(const LanguageKey &key)
{
    if (key.context() == dynamicStringsContext)
        return false;

    return true;
}

void LanguagesManager::sendLanguages()
{

}
