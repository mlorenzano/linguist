#pragma once

#include <QString>

class TranslatableMessage
{
public:
    TranslatableMessage();
    TranslatableMessage(const QString &source, const QString &translation);
    TranslatableMessage(const TranslatableMessage &other);

    TranslatableMessage &operator=(const TranslatableMessage &other);
    bool operator==(const TranslatableMessage &other) const;
    inline bool operator!=(const TranslatableMessage &other) const { return !operator==(other); }

    QString source() const { return m_source; }
    void setSource(const QString &source);

    QString translation() const { return m_translation; }
    void setTranslation(const QString &translation);

private:
    QString m_source;
    QString m_translation;
};
