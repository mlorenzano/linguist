#pragma once

#include "languagekey.h"
#include "standardlanguage.h"
#include "translatablelanguage.h"
#include "abstractlanguagemodule.h"
#include <QObject>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QMessageBox>


class QTextStream;

const QString dynamicStringsContext = "$$DynamicStrings$$";
const QString eventsHandlerContext = "$$EventsHandler$$";

class LanguagesManager : public QObject  //, public ElcoIdeCoreModule, public CompilableModuleInterface
{
    Q_OBJECT
public:
    static LanguagesManager *instance();
    ~LanguagesManager();

    StandardLanguage* standard();
    QList<TranslatableLanguage*> translatables();
    QList<AbstractLadderLanguage*> languages();

    bool addTranslatable(const QString &name);
    void removeTranslatable(const QString &name);

    AbstractLadderLanguage* currentLanguage();
    bool setCurrentLanguage(AbstractLadderLanguage *language);
    bool setCurrentLanguage(const int index);

    QString workingPath() const { return m_workingPath; }
    QString absoluteManagerPath() const;

    // Message handling
    void setMessage(const LanguageKey &key, const QString &message);
    void setMessage(const QString &context, const QString &id, const QString &message);
    void setMessage(const QString &context, const QString &id, const QString &optional, const QString &message);
    int messagesCount();

    // Languages utils
    bool languageExists(const QString &name);

    // Compile's functions
    void clearGraphicObjects();
    inline QHash<LanguageKey, int> graphicObjects() const { return m_graphic_objs; }
    void setGraphicObjects(QHash<LanguageKey, int> graphic_objs);
    void setGraphicObject(const LanguageKey &key, int id_graphic_obj);
    void setGraphicObject(const QString &context, const QString &id, int id_graphic_obj);
    void setGraphicObject(const QString &context, const QString &id, const QString &optional, int id_graphic_obj);

    // Modules
    bool registerModule(AbstractLanguageModule *mdl);
    void initializeModulesMessages();

    // Export
    enum ExportErrors {
        NoErrors,
        ExportPathNotFound,
        EmptyGraphicObjs,
        LanguageNotFound,
        LanguageError,
        Warnings,

        NotConnected,
        FirmwareError,
        ProjectNotAligned
    };
    ExportErrors exportLanguages(const QString &path);
    ExportErrors exportLanguage(const QString &path, const QString &language);
    QStringList exportWarnings() const;
    ExportErrors checkExport();

    // Mnemonic
    void setRebuild();
    QString toMnemonic();

    // Send files
    ExportErrors requestSendLanguages();

    void addLngFilesForLpuOrDownload(const QString &path);

    /*!
     * Functions inherited from ElcoIdeCoreModule
     */
    void init() {}
    void clear();
    bool save();
    bool saveConfigurationOnFile();
    bool load();
    QString errorDescription() const;
    void setWorkingPath(const QString &path);
    void setWorkingTempPath(const QString &path);
    QString moduleName() const { return QString("LanguagesManager"); }
    bool modified() const;
    void setModified(const bool b);
    QString version() const;
    void connectEditedSlot(QObject *receiver, const char *slot);

    static bool handlesNewLines(const LanguageKey &key);

signals:
    void languageAdded(const QString &name);
    void languageRemoved(const QString &name);
    void exportRequested();
    void loaded();
    void changed();
    void changedFromLastBuild();

public slots:
    void sendLanguages();

private slots:
    void symbolRemoved(int id);
    void symbolChanged(int oldId, int newId);

protected:
    LanguagesManager(QObject *parent = 0);
    static LanguagesManager *theInstance;

    bool addTranslatable(const QString &name, int symbol_id);
    void adjustTranslatableSymbol();

    //ExportErrors streamExport(QTextStream &stream, AbstractLadderLanguage *lng);

private:
    // Manager's variables
    StandardLanguage *m_standard;
    QList<TranslatableLanguage*> m_translatables;
    int m_current_language;
    QString m_workingPath;
    QString m_workingTempPath;
    QStringList m_export_warnings;
    QStringList m_filesToTransmit;
    bool m_dirty;

    // Compile's variables
    QHash<LanguageKey, int> m_graphic_objs;

    // Modules
    QList<AbstractLanguageModule*> m_modules;

    // Functions
    ExportErrors fileExport(QFile *file, const QString &filename, AbstractLadderLanguage *lng);

    // Gestione retrocompatibilita.
    QString checkFileVersion();
    bool saveToBetaVersion();           // Salvataggio versione beta
    bool loadFromBetaVersion();         // Caricamento da versione beta
    bool saveCurrent();                 // Salvataggio versione corrente
    bool loadCurrent();                 // Caricamento da versione corrente

    bool setModifiedFromLastBuild(const bool b);
};
