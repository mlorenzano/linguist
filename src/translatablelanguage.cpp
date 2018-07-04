#include "translatablelanguage.h"
#include "languagekey.h"
#include "languagesmanager.h"
#include <QDir>
#include <QFile>
#include "qdom.h"
#include "qxml.h"
#include <QDebug>

const QString languagefile_version              = "0.1";

TranslatableLanguage::TranslatableLanguage() :
    m_symbol_id(-1),
    m_name(QString()),
    m_messages(QHash<LanguageKey, TranslatableMessage>()),
    m_manager(0)
{
}

TranslatableLanguage::TranslatableLanguage(const TranslatableLanguage &other) :
    m_symbol_id(-1),
    m_name(other.m_name),
    m_messages(other.m_messages),
    m_manager(other.m_manager)
{
}

TranslatableLanguage::~TranslatableLanguage()
{
}

TranslatableLanguage& TranslatableLanguage::operator=(const TranslatableLanguage &other)
{
    m_name = other.m_name;
    m_messages = other.m_messages;
    m_manager = other.m_manager;
    m_symbol_id = other.m_symbol_id;
    return *this;
}

bool TranslatableLanguage::operator==(const TranslatableLanguage &other) const
{
    return m_name == other.m_name;
}

void TranslatableLanguage::setName(const QString &name)
{
    if (m_name != name)
        m_name = name;
}

bool TranslatableLanguage::contextExists(const QString &context) const
{
    QHash<LanguageKey, TranslatableMessage>::const_iterator  i;
    for (i = m_messages.constBegin(); i != m_messages.constEnd(); ++i) {
        LanguageKey key = i.key();
        if (key.context() == context)
            return true;
    }

    return false;
}

QString TranslatableLanguage::message(const LanguageKey &key) const
{
    if (!m_messages.keys().contains(key))
        return QString();

    TranslatableMessage msg = m_messages.value(key);
    return msg.source();
}

QString TranslatableLanguage::message(const QString &context, const QString &id) const
{
    LanguageKey key(context, id);
    return message(key);
}

QString TranslatableLanguage::message(const QString &context, const QString &id, const QString &optional) const
{
    LanguageKey key(context, id, optional);
    return message(key);
}

void TranslatableLanguage::setMessage(const LanguageKey &key, const QString &message)
{
    /*
     * If there is already an item with the key, that item's value is replaced with message.
     */
    if (!m_messages.keys().contains(key)) {
        TranslatableMessage msg(message, QString());
        m_messages.insert(key, msg);
    }
}

void TranslatableLanguage::setMessage(const QString &context, const QString &id, const QString &message)
{
    LanguageKey key(context, id);
    setMessage(key, message);
}

void TranslatableLanguage::setMessage(const QString &context, const QString &id, const QString &optional, const QString &message)
{
    LanguageKey key(context, id, optional);
    setMessage(key, message);
}

QString TranslatableLanguage::translation(const LanguageKey &key) const
{
    if (!m_messages.keys().contains(key))
        return QString();

    TranslatableMessage msg = m_messages.value(key);
    return msg.translation();
}

QString TranslatableLanguage::translation(const QString &context, const QString &id) const
{
    LanguageKey key(context, id);
    return translation(key);
}

QString TranslatableLanguage::translation(const QString &context, const QString &id, const QString &optional) const
{
    LanguageKey key(context, id, optional);
    return translation(key);
}

void TranslatableLanguage::setTranslation(const LanguageKey &key, const QString &message)
{
    /*
     * If there is already an item with the key, that item's value is replaced with message.
     */
    TranslatableMessage msg;
    if (m_messages.keys().contains(key)) {
        msg = m_messages.value(key);
    }
    StandardLanguage *standard = m_manager->standard();
    msg.setSource(standard->message(key));
    msg.setTranslation(message);
    m_messages.insert(key, msg);
}

void TranslatableLanguage::setTranslation(const QString &context, const QString &id, const QString &message)
{
    LanguageKey key(context, id);
    setTranslation(key, message);
}

void TranslatableLanguage::setTranslation(const QString &context, const QString &id, const QString &optional, const QString &message)
{
    LanguageKey key(context, id, optional);
    setTranslation(key, message);
}

void TranslatableLanguage::setTranslation(const LanguageKey &key, const TranslatableMessage &message)
{
    // Insert the message
    m_messages.insert(key, message);
}

bool TranslatableLanguage::isTranslationUpdate(const LanguageKey &key)
{
    if (!m_messages.keys().contains(key))
        return false;

    StandardLanguage *standard = m_manager->standard();
    if (standard->message(key) != message(key))
        return false;

    return true;
}

void TranslatableLanguage::clear()
{
    m_messages.clear();
}

bool TranslatableLanguage::load()
{
    if (!m_manager) {
        qDebug() << "TranslatableLanguage::load(): manager not initialized";
        return false;
    }

    QDir dir(m_manager->absoluteManagerPath());
    if (!dir.exists()) {
        qDebug() << "TranslatableLanguage::load(): working path not valid";
        return false;
    }

    QString fileName = QString("%1.xml").arg(m_name);
    QFile file(dir.absolutePath() + "/" + fileName);
    if (!file.exists()) {
        qDebug() << "TranslatableLanguage::load(): file not exists";
        return false;
    }

    // Clear the map
    clear();

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "TranslatableLanguage::load(): error open(R) file" << file.fileName();
        return false;
    }

    // Parse XML file
    QDomDocument doc;
    QXmlInputSource source(&file);
    QXmlSimpleReader reader;
    if (!doc.setContent(&source, &reader)) {
        qDebug() << "TranslatableLanguage::load(): could not parse content of file" << file.fileName();
        file.close();
        return false;
    }
    file.close();

    const QString context_tag = QString("context");
    const QString optional_tag = QString("optional");
    const QString id_tag = QString("id");
    const QString source_tag = QString("source");
    const QString translation_tag = QString("translation");
    const QString name_attribute = QString("name");

    QDomNodeList contexts = doc.elementsByTagName(context_tag);
    for (int i = 0; i < contexts.size(); i++) {
        QDomElement context = contexts.at(i).toElement();
        QString context_name = context.attribute(name_attribute, QString());
        if (context_name.isEmpty()) {
            qDebug() << "TranslatableLanguage::load(): context name contains empty value" << file.fileName();
            continue;
        }

        QDomNodeList optionals = context.elementsByTagName(optional_tag);
        for (int j = 0; j < optionals.size(); j++) {
            QDomElement optional = optionals.at(j).toElement();
            QString optional_name = optional.attribute(name_attribute, QString());

            QDomNodeList ids = optional.elementsByTagName(id_tag);
            for (int k = 0; k < ids.size(); k++) {
                QDomElement id = ids.at(k).toElement();
                QString id_name = id.attribute(name_attribute, QString());
                if (id_name.isEmpty()) {
                    qDebug() << "TranslatableLanguage::load(): id name contains empty value" << file.fileName();
                    continue;
                }

                QDomElement source = id.elementsByTagName(source_tag).at(0).toElement();
                QDomElement translation = id.elementsByTagName(translation_tag).at(0).toElement();

                LanguageKey key(context_name, id_name, optional_name);
                TranslatableMessage msg(source.text(), translation.text());

                // Insert the message
                m_messages.insert(key, msg);
            }
        }
    }

    return true;
}

bool TranslatableLanguage::save()
{
    if (!m_manager) {
        qDebug() << "TranslatableLanguage::save(): manager not initialized";
        return false;
    }

    QDir dir(m_manager->absoluteManagerPath());
    if (!dir.exists()) {
        qDebug() << "TranslatableLanguage::save(): working path not valid";
        return false;
    }

    QString fileName = QString("%1.xml").arg(m_name);
    QFile file(dir.absolutePath() + "/" + fileName);
    if (file.exists())
        file.remove();

    // Create the Document
    QDomDocument doc(fileName);
    QList<QDomElement*> contexts;

    const QString language_tag = QString("language");
    const QString version_attribute = QString("version");
    const QString context_tag = QString("context");
    const QString optional_tag = QString("optional");
    const QString id_tag = QString("id");
    const QString source_tag = QString("source");
    const QString translation_tag = QString("translation");
    const QString name_attribute = QString("name");

    // Build the root element
    QDomElement language = doc.createElement(language_tag);
    language.setAttribute(version_attribute, languagefile_version);

    // Cycle all the messages and create the context's structures
    QHash<LanguageKey, TranslatableMessage>::const_iterator  i;
    for (i = m_messages.constBegin(); i != m_messages.constEnd(); ++i) {
        LanguageKey key = i.key();
        TranslatableMessage value = i.value();
        // Check if the context already exist
        QDomElement *context = new QDomElement();
        foreach (QDomElement *ct, contexts) {
            QString name = ct->attribute(name_attribute, QString());
            if (!name.isEmpty() && name == key.context()) {
                context = ct;
                break;
            }
        }

        // If the context doesn't exist, create a new one
        if (context->isNull()) {
            (*context) = doc.createElement(context_tag);
            context->setAttribute(name_attribute, key.context());
            contexts << context;
        }

        // Check optional
        QDomNode old_opt;
        QDomElement opt;
        QDomNodeList opts = context->elementsByTagName(optional_tag);
        for (int j = 0; j < opts.size(); j++) {
            QDomElement op = opts.at(j).toElement();
            if (op.attribute(name_attribute, QString()) == key.optional()) {
                opt = op;
                old_opt = opts.at(j);
            }
        }

        if (opt.isNull()) {
            opt = doc.createElement(optional_tag);
            opt.setAttribute(name_attribute, key.optional());
        }

        // Create the source's node
        QDomElement source = doc.createElement(source_tag);
        QDomText ts = doc.createTextNode(value.source());
        source.appendChild(ts);
        // Create the translation's node
        QDomElement translation = doc.createElement(translation_tag);
        QDomText tr = doc.createTextNode(value.translation());
        translation.appendChild(tr);

        // Create the id's node
        QDomElement id = doc.createElement(id_tag);
        id.setAttribute(name_attribute, key.id());
        id.appendChild(source);
        id.appendChild(translation);

        // Add the id to context
        opt.appendChild(id);

        if (!old_opt.isNull())
            context->replaceChild(old_opt, opt);
        else
            context->appendChild(opt);
    }

    // Append all the context's nodes to the doc
    foreach (QDomElement *ct, contexts) {
        language.appendChild(*ct);
        delete ct;
    }

    // Clear the list's data
    contexts.clear();

    // Setup the doc
    doc.appendChild(language);

    // Save the doc to a file
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        qDebug() << "TranslatableLanguage::save(): error open(W) file" << file.fileName();
        return false;
    }

    QByteArray xml = doc.toByteArray();
    file.write(xml);
    file.close();

    return true;
}

void TranslatableLanguage::setManager(LanguagesManager *manager)
{
    m_manager = manager;
}

int TranslatableLanguage::size()
{
    return m_messages.size();
}

QList<LanguageKey> TranslatableLanguage::keys() const
{
    return m_messages.keys();
}

QStringList TranslatableLanguage::contexts() const
{
    QStringList list;
    QHash<LanguageKey, TranslatableMessage>::const_iterator  i;
    for (i = m_messages.constBegin(); i != m_messages.constEnd(); ++i) {
        bool found = false;
        LanguageKey key = i.key();
        foreach (QString s, list) {
            if (s == key.context()) {
                found = true;
                break;
            }
        }

        // If the context doesn't exist, create a new one
        if (!found) {
            list << key.context();
        }
    }

    return list;
}

QString TranslatableLanguage::displayed(const LanguageKey &key) const
{
    StandardLanguage *standard = m_manager->standard();
    QString msg = translation(key);
    if (msg.isEmpty() ||
            !standard->translatable(key))
        msg = standard->message(key);
    return msg;
}

QString TranslatableLanguage::displayed(const QString &context, const QString &id) const
{
    LanguageKey key(context, id);
    return displayed(key);
}

QString TranslatableLanguage::displayed(const QString &context, const QString &id, const QString &optional) const
{
    LanguageKey key(context, id, optional);
    return displayed(key);
}
