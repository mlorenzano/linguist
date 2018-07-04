#pragma once

#include <QString>

class StandardMessage
{
public:
    StandardMessage();
    StandardMessage(const QString &source, bool translatable = true);
    StandardMessage(const StandardMessage &other);

    StandardMessage &operator=(const StandardMessage &other);
    bool operator==(const StandardMessage &other) const;
    inline bool operator!=(const StandardMessage &other) const { return !operator==(other); }

    QString source() const { return m_source; }
    void setSource(const QString &source);

    bool translatable() const { return m_translatable; }
    void setTranslatable(bool translatable);

private:
    QString m_source;
    bool m_translatable;
};

