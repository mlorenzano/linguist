#include "standardmessage.h"

StandardMessage::StandardMessage() :
    m_source(QString()),
    m_translatable(true)
{
}

StandardMessage::StandardMessage(const QString &source, bool translatable) :
    m_source(source),
    m_translatable(translatable)
{
}

StandardMessage::StandardMessage(const StandardMessage &other) :
    m_source(other.m_source),
    m_translatable(other.m_translatable)
{
}

StandardMessage& StandardMessage::operator=(const StandardMessage &other)
{
    m_source = other.m_source;
    m_translatable = other.m_translatable;
    return *this;
}

bool StandardMessage::operator==(const StandardMessage &other) const
{
    return m_source == other.m_source &&
            m_translatable == other.m_translatable;
}

void StandardMessage::setSource(const QString &source)
{
    if (m_source != source)
        m_source = source;
}

void StandardMessage::setTranslatable(bool translatable)
{
    if (m_translatable != translatable)
        m_translatable = translatable;
}
