#pragma once

#include <QString>
#include <QHash>

class LanguageKey
{
public:
    LanguageKey();
    LanguageKey(const QString &context, const QString &id);
    LanguageKey(const QString &context, const QString &id, const QString &optional);
    LanguageKey(const LanguageKey &other);

    LanguageKey &operator=(const LanguageKey &other);
    bool operator==(const LanguageKey &other) const;
    inline bool operator!=(const LanguageKey &other) const { return !operator==(other); }

    inline QString context() const { return m_context; }
    void setContext(const QString &context);

    inline QString id() const { return m_id; }
    void setId(const QString &id);

    inline QString optional() const { return m_optional; }
    void setOptional(const QString &optional);

    QString toString() const;
    bool fromString(const QString &str);

private:
    QString m_context;
    QString m_id;
    QString m_optional;

};

//inline bool operator==(const LanguageKey &l1, const LanguageKey &l2)
//{
//    return (l1.context() == l2.context() &&
//            l1.id() == l2.id());
//}

inline uint qHash(const LanguageKey &key)
{
    return (qHash(key.context()) ^ qHash(key.id()) ^ qHash(key.optional()));
}
