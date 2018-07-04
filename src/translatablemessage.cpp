#include "translatablemessage.h"

TranslatableMessage::TranslatableMessage() :
    m_source(QString()),
    m_translation(QString())
{
}

TranslatableMessage::TranslatableMessage(const QString &source, const QString &translation) :
    m_source(source),
    m_translation(translation)
{
}

TranslatableMessage::TranslatableMessage(const TranslatableMessage &other) :
    m_source(other.m_source),
    m_translation(other.m_translation)
{
}

TranslatableMessage& TranslatableMessage::operator=(const TranslatableMessage &other)
{
    m_source = other.m_source;
    m_translation = other.m_translation;
    return *this;
}

bool TranslatableMessage::operator==(const TranslatableMessage &other) const
{
    return m_source == other.m_source &&
            m_translation == other.m_translation;
}

void TranslatableMessage::setSource(const QString &source)
{
    if (m_source != source)
        m_source = source;
}

void TranslatableMessage::setTranslation(const QString &translation)
{
    if (m_translation != translation)
        m_translation = translation;
}
