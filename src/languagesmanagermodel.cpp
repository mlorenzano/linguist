#include "languagesmanagermodel.h"
#include "languagekey.h"
#include <QBrush>
#include <QFont>
#include "standardlanguage.h"

// Consts
const QColor empty_color = QColor(253, 177, 94);
const QColor not_update_color = QColor(156, 250, 112);
const QColor invalid_dynamic_color = QColor(255, 117, 120);

LanguagesManagerModel::LanguagesManagerModel() :
    QAbstractTableModel(),
    m_manager(LanguagesManager::instance()),
    m_context(QString()),
    m_languages(QStringList())
{
    connect(m_manager, SIGNAL(languageAdded(QString)),
            this, SLOT(managerLanguageAdded(QString)));
    connect(m_manager, SIGNAL(languageRemoved(QString)),
            this, SLOT(managerLanguageRemoved(QString)));

    QList<AbstractLadderLanguage*> languages = m_manager->languages();
    for (int i = 0; i < languages.size(); i++) {
        AbstractLadderLanguage *lng = languages.at(i);
        m_languages << lng->name();
    }

    init();
}

void LanguagesManagerModel::init()
{
    // Begins a model reset operation
    beginResetModel();

    StandardLanguage *standard = m_manager->standard();
    QList<LanguageKey> keys = standard->keys();
    QList<AbstractLadderLanguage*> languages = m_manager->languages();

    // Rows
    m_rowmap.clear();
    int id = 0;
    for (int i = 0; i < keys.size(); i++) {
        LanguageKey key = keys.at(i);
        if ((m_context.isEmpty() || m_context == key.context()) &&
                m_manager->standard()->translatable(key))
            m_rowmap.insert(id++, key);
    }

    // Cols
    m_columnmap.clear();
    id = 0;
    for (int i = 0; i < languages.size(); i++) {
        AbstractLadderLanguage *lng = languages.at(i);
        if (m_languages.contains(lng->name()))
            m_columnmap.insert(id++, i);
    }

    // Completes a model reset operation.
    endResetModel();
}

int LanguagesManagerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowmap.size();
}

int LanguagesManagerModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnmap.size();
}

QVariant LanguagesManagerModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        LanguageKey key = m_rowmap.value(index.row());
        int column = m_columnmap.value(index.column());
        AbstractLadderLanguage *lng = m_manager->languages().at(column);
        QString message;
        if (lng->type() == StandardLanguage::StandardLanguageType) {
            message = lng->message(key);
        }
        else if (lng->type() == TranslatableLanguage::TranslatableLanguageType) {
            TranslatableLanguage *tr = (TranslatableLanguage*)(lng);
            message = tr->translation(key);
        }

        if (!message.isEmpty())
            return escaped(message);
    }
    else if (role == Qt::BackgroundRole) {
        LanguageKey key = m_rowmap.value(index.row());
        int column = m_columnmap.value(index.column());
        AbstractLadderLanguage *lng = m_manager->languages().at(column);
        if (lng->type() == TranslatableLanguage::TranslatableLanguageType) {
            TranslatableLanguage *tr = (TranslatableLanguage*)(lng);
            if (tr->translation(key).isEmpty())
                return QBrush(empty_color);
            else if (!tr->isTranslationUpdate(key))
                return QBrush(not_update_color);
            else {
               /* if (key.context() == dynamicStringsContext &&
                        tr->translation(key).length() != m_manager->standard()->message(key).length() &&
                    !FirmwareManager::instance()->currentVersion()
                     .hasFeature(FirmwareVersion::LanguagesExport)) {
                    return QBrush(invalid_dynamic_color);
                }*/
            }
        }
    }
    else if (role == Qt::FontRole) {
        int column = m_columnmap.value(index.column());
        AbstractLadderLanguage *lng = m_manager->languages().at(column);
        if (lng->type() == StandardLanguage::StandardLanguageType) {
            QFont italicFont;
            italicFont.setItalic(true);
            return italicFont;
        }
    }

    return QVariant::Invalid;
}

QString LanguagesManagerModel::escaped(const QString &message) const
{
    QString newMessage {message};
    if (newMessage.contains('\n'))
        newMessage.replace('\n', "\\n");

    return newMessage;
}

QVariant LanguagesManagerModel::headerData(int section,
                                           Qt::Orientation orientation,
                                           int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            int column = m_columnmap.value(section);
            AbstractLadderLanguage *lng = m_manager->languages().at(column);
            return lng->name();
        }
        else if (orientation == Qt::Vertical) {
            return QString::number(section + 1);
        }
    }

    return QVariant::Invalid;
}

bool LanguagesManagerModel::setData(const QModelIndex & index,
                                    const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        int column = m_columnmap.value(index.column());
        AbstractLadderLanguage *lng = m_manager->languages().at(column);
        if (lng->type() == StandardLanguage::StandardLanguageType)
            return false;

        if (lng->type() == TranslatableLanguage::TranslatableLanguageType) {
            LanguageKey key = m_rowmap.value(index.row());
            TranslatableLanguage *tr = (TranslatableLanguage*)(lng);

            tr->setTranslation(key, value.toString());
        }

        emit dataChanged(index, index);
    }

    return true;
}

Qt::ItemFlags LanguagesManagerModel::flags(const QModelIndex & index) const
{
    int column = m_columnmap.value(index.column());
    AbstractLadderLanguage *lng = m_manager->languages().at(column);
    if (lng->type() == TranslatableLanguage::TranslatableLanguageType)
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QString LanguagesManagerModel::context() const
{
    return m_context;
}

void LanguagesManagerModel::setContext(const QString &context)
{
    if (m_context != context) {
        m_context = context;
        // change datas
        init();
    }
}

QStringList LanguagesManagerModel::languages() const
{
    return m_languages;
}

void LanguagesManagerModel::setLanguages(const QStringList &languages)
{
    if (m_languages != languages) {
        m_languages = languages;
        // change datas
        init();
    }
}

QString LanguagesManagerModel::standardDataFromIndex(const QModelIndex &index)
{
    QModelIndex stdIdx = createIndex(index.row(), 0);
    return data(stdIdx, Qt::DisplayRole).toString();
}

void LanguagesManagerModel::managerLanguageAdded(const QString &name)
{
    m_languages << name;
}

void LanguagesManagerModel::managerLanguageRemoved(const QString &name)
{
    m_languages.removeAll(name);
}

LanguageKey LanguagesManagerModel::key(const QModelIndex &index) const
{
    return m_rowmap.value(index.row());
}
