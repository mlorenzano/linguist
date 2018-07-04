#include "standardlanguage.h"
#include "languagekey.h"
#include "languagesmanager.h"
#include <QDir>
#include <QFile>
#include <QDomDocument>
#include <QXmlSimpleReader>
#include <QDebug>

const QString languagefile_version              = "0.1";

StandardLanguage::StandardLanguage() :
    m_name(QString()),
    m_messages(QHash<LanguageKey, StandardMessage>()),
    m_manager(0)
{
}

StandardLanguage::StandardLanguage(const StandardLanguage &other) :
    m_name(other.m_name),
    m_messages(other.m_messages),
    m_manager(other.m_manager)
{
}

StandardLanguage::~StandardLanguage()
{
}

StandardLanguage& StandardLanguage::operator=(const StandardLanguage &other)
{
    m_name = other.m_name;
    m_messages = other.m_messages;
    m_manager = other.m_manager;
    return *this;
}

bool StandardLanguage::operator==(const StandardLanguage &other) const
{
    return m_name == other.m_name;
}

void StandardLanguage::setName(const QString &name)
{
    if (m_name != name)
        m_name = name;
}

bool StandardLanguage::contextExists(const QString &context) const
{
    QHash<LanguageKey, StandardMessage>::const_iterator  i;
    for (i = m_messages.constBegin(); i != m_messages.constEnd(); ++i) {
        LanguageKey key = i.key();
        if (key.context() == context)
            return true;
    }

    return false;
}

QString StandardLanguage::message(const LanguageKey &key) const
{
    if (!m_messages.keys().contains(key))
        return QString();

    StandardMessage msg = m_messages.value(key);
    return msg.source();
}

QString StandardLanguage::messageTranslatable(const LanguageKey &key) const
{
    if (!m_messages.keys().contains(key) ||
            !m_messages.value(key).translatable())
        return {};

    return m_messages.value(key).source();
}

QString StandardLanguage::message(const QString &context, const QString &id) const
{
    LanguageKey key(context, id);
    return message(key);
}

QString StandardLanguage::message(const QString &context, const QString &id, const QString &optional) const
{
    LanguageKey key(context, id, optional);
    return message(key);
}

void StandardLanguage::setMessage(const LanguageKey &key, const QString &message)
{
    /*
     * If there is already an item with the key, that item's value is replaced with message.
     */
    StandardMessage msg;
    msg.setSource(message);
    m_messages.insert(key, msg);
}

void StandardLanguage::setMessage(const QString &context, const QString &id, const QString &message)
{
    LanguageKey key(context, id);
    setMessage(key, message);
}

void StandardLanguage::setMessage(const QString &context, const QString &id, const QString &optional, const QString &message)
{
    LanguageKey key(context, id, optional);
    setMessage(key, message);
}

bool StandardLanguage::translatable(const LanguageKey &key)
{
    if (!m_messages.keys().contains(key))
        return false;

    StandardMessage msg = m_messages.value(key);
    return msg.translatable();
}

bool StandardLanguage::translatable(const QString &context, const QString &id)
{
    LanguageKey key(context, id);
    return translatable(key);
}

bool StandardLanguage::translatable(const QString &context, const QString &id, const QString &optional)
{
    LanguageKey key(context, id, optional);
    return translatable(key);
}

void StandardLanguage::setTranslatable(const LanguageKey &key, bool translatable)
{
    StandardMessage msg;
    if (m_messages.keys().contains(key)) {
        msg = m_messages.value(key);
    }
    msg.setTranslatable(translatable);
    m_messages.insert(key, msg);
}

void StandardLanguage::setTranslatable(const QString &context, const QString &id, bool translatable)
{
    LanguageKey key(context, id);
    setTranslatable(key, translatable);
}

void StandardLanguage::setTranslatable(const QString &context, const QString &id, const QString &optional, bool translatable)
{
    LanguageKey key(context, id, optional);
    setTranslatable(key, translatable);
}

void StandardLanguage::clear()
{
    m_messages.clear();
}

bool StandardLanguage::load()
{
    if (!m_manager) {
        qDebug() << "StandardLanguage::load(): manager not initialized";
        return false;
    }

    QDir dir(m_manager->absoluteManagerPath());
    if (!dir.exists()) {
        qDebug() << "StandardLanguage::load(): working path not valid";
        return false;
    }

    QString fileName = QString("%1.xml").arg(m_name);
    QFile file(dir.absolutePath() + "/" + fileName);
    if (!file.exists()) {
        qDebug() << "StandardLanguage::load(): file not exists";
        return false;
    }

    // Clear the map
    clear();

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "StandardLanguage::load(): error open(R) file" << file.fileName();
        return false;
    }

    // Parse XML file
    QDomDocument doc;
    QXmlInputSource source(&file);
    QXmlSimpleReader reader;
    if (!doc.setContent(&source, &reader)) {
        qDebug() << "StandardLanguage::load(): could not parse content of file" << file.fileName();
        file.close();
        return false;
    }
    file.close();

    const QString context_tag = QString("context");
    const QString optional_tag = QString("optional");
    const QString id_tag = QString("id");
    const QString message_tag = QString("message");
    const QString name_attribute = QString("name");
    const QString translatable_attribute = QString("tr");

    QDomNodeList contexts = doc.elementsByTagName(context_tag);
    for (int i = 0; i < contexts.size(); i++) {
        QDomElement context = contexts.at(i).toElement();
        QString context_name = context.attribute(name_attribute, QString());
        if (context_name.isEmpty()) {
            qDebug() << "StandardLanguage::load(): context name contains empty value" << file.fileName();
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
                    qDebug() << "StandardLanguage::load(): id name contains empty value" << file.fileName();
                    continue;
                }

                QDomElement message = id.elementsByTagName(message_tag).at(0).toElement();
                bool translatable = true;
                if (message.hasAttribute(translatable_attribute) &&
                        message.attribute(translatable_attribute, QString("true")) == "false")
                    translatable = false;

                LanguageKey key(context_name, id_name, optional_name);
                StandardMessage msg(message.text(), translatable);

                // Insert the message
                m_messages.insert(key, msg);
            }
        }
    }

    return true;
}

bool StandardLanguage::save()
{
    if (!m_manager) {
        qDebug() << "StandardLanguage::save(): manager not initialized";
        return false;
    }

    QDir dir(m_manager->absoluteManagerPath());
    if (!dir.exists()) {
        qDebug() << "StandardLanguage::save(): working path not valid";
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
    const QString message_tag = QString("message");
    const QString name_attribute = QString("name");
    const QString translatable_attribute = QString("tr");

    // Build the root element
    QDomElement language = doc.createElement(language_tag);
    language.setAttribute(version_attribute, languagefile_version);

    // Cycle all the messages and create the context's structures
    QHash<LanguageKey, StandardMessage>::const_iterator  i;
    for (i = m_messages.constBegin(); i != m_messages.constEnd(); ++i) {
        LanguageKey key = i.key();
        StandardMessage value = i.value();
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

        // Create the message's node
        QDomElement message = doc.createElement(message_tag);
        QDomText text = doc.createTextNode(value.source());
        if (!value.translatable())
            message.setAttribute(translatable_attribute, QString("false"));
        message.appendChild(text);

        // Create the id's node
        QDomElement id = doc.createElement(id_tag);
        id.setAttribute(name_attribute, key.id());
        id.appendChild(message);

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
        qDebug() << "StandardLanguage::save(): error open(W) file" << file.fileName();
        return false;
    }

    QByteArray xml = doc.toByteArray();
    file.write(xml);
    file.close();

    return true;
}

void StandardLanguage::setManager(LanguagesManager *manager)
{
    m_manager = manager;
}

int StandardLanguage::size()
{
    return m_messages.size();
}

QList<LanguageKey> StandardLanguage::keys() const
{
    return m_messages.keys();
}

QStringList StandardLanguage::contexts() const
{
    QStringList list;
    QHash<LanguageKey, StandardMessage>::const_iterator  i;
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

QString StandardLanguage::displayed(const LanguageKey &key) const
{
    QString msg = message(key);
    return msg;
}

QString StandardLanguage::displayed(const QString &context, const QString &id) const
{
    LanguageKey key(context, id);
    return displayed(key);
}

QString StandardLanguage::displayed(const QString &context, const QString &id, const QString &optional) const
{
    LanguageKey key(context, id, optional);
    return displayed(key);
}
