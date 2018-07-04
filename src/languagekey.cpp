#include "languagekey.h"
#include <QStringList>

LanguageKey::LanguageKey() :
    m_context(QString()),
    m_id(QString()),
    m_optional(QString())
{
}

LanguageKey::LanguageKey(const QString &context, const QString &id) :
    m_context(context),
    m_id(id),
    m_optional(QString())
{
}

LanguageKey::LanguageKey(const QString &context, const QString &id, const QString &optional) :
    m_context(context),
    m_id(id),
    m_optional(optional)
{
}

LanguageKey::LanguageKey(const LanguageKey &other) :
    m_context(other.m_context),
    m_id(other.m_id),
    m_optional(other.m_optional)
{
}

LanguageKey& LanguageKey::operator=(const LanguageKey &other)
{
    m_context = other.m_context;
    m_id = other.m_id;
    m_optional = other.m_optional;
    return *this;
}

bool LanguageKey::operator==(const LanguageKey &other) const
{
    return m_context == other.m_context &&
            m_id == other.m_id &&
            m_optional == other.m_optional;
}

void LanguageKey::setContext(const QString &context)
{
    if (m_context != context)
        m_context = context;
}

void LanguageKey::setId(const QString &id)
{
    if (m_id != id)
        m_id = id;
}

void LanguageKey::setOptional(const QString &optional)
{
    if (m_optional != optional)
        m_optional = optional;
}

QString LanguageKey::toString() const
{
    return QString("ct=\"%1\" id=\"%2\" op=\"%3\"").arg(m_context).arg(m_id).arg(m_optional);
}

bool LanguageKey::fromString(const QString &str)
{
    QString key, value, param;
    QRegExp reValue("((ct|id|op)=\"+[\\S]+\"+)");
    int pos = 0;
    bool ok = false;

    m_context.clear();
    m_id.clear();
    m_optional.clear();

    while ((pos = reValue.indexIn(str, pos)) != -1) {
        ok = true;
        param = reValue.cap(1);
        pos += reValue.matchedLength();
        key = param.split("=").first();
        value = param.split("=").last().replace("\"", "");

        if (key == "ct")
            m_context = value;

        else if (key == "id")
            m_id = value;

        else if (key == "op")
            m_optional = value;

        else {
            m_context.clear();
            m_id.clear();
            m_optional.clear();
            return false;
        }
    }

    return ok;
}

