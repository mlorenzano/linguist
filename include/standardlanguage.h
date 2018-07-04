#pragma once

#include "abstractladderlanguage.h"
#include "standardmessage.h"

#include <QString>
#include <QHash>

class StandardLanguage : public AbstractLadderLanguage
{
public:
    StandardLanguage();
    StandardLanguage(const StandardLanguage &other);
    ~StandardLanguage();

    StandardLanguage &operator=(const StandardLanguage &other);
    bool operator==(const StandardLanguage &other) const;

    inline QString name() const { return m_name; }
    void setName(const QString &name);

    static const int StandardLanguageType = 0;
    inline int type() { return StandardLanguageType; }

    bool contextExists(const QString &context) const;

    QString message(const LanguageKey &key) const;
    QString messageTranslatable(const LanguageKey &key) const;
    QString message(const QString &context, const QString &id) const;
    QString message(const QString &context, const QString &id, const QString &optional) const;
    void setMessage(const LanguageKey &key, const QString &message);
    void setMessage(const QString &context, const QString &id, const QString &message);
    void setMessage(const QString &context, const QString &id, const QString &optional, const QString &message);
    bool translatable(const LanguageKey &key);
    bool translatable(const QString &context, const QString &id);
    bool translatable(const QString &context, const QString &id, const QString &optional);
    void setTranslatable(const LanguageKey &key, bool translatable);
    void setTranslatable(const QString &context, const QString &id, bool translatable);
    void setTranslatable(const QString &context, const QString &id, const QString &optional, bool translatable);

    void clear();
    bool load();
    bool save();

    inline LanguagesManager *manager() const { return m_manager; }
    void setManager(LanguagesManager *manager);

    int size();

    QList<LanguageKey> keys() const;

    QStringList contexts() const;

    QString displayed(const LanguageKey &key) const;
    QString displayed(const QString &context, const QString &id) const;
    QString displayed(const QString &context, const QString &id, const QString &optional) const;

private:
    QString m_name;
    QHash<LanguageKey, StandardMessage> m_messages;
    LanguagesManager *m_manager;

};
