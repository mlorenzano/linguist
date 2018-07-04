#pragma once

#include "abstractladderlanguage.h"
#include "translatablemessage.h"

#include <QString>
#include <QHash>

class TranslatableLanguage : public AbstractLadderLanguage
{
    friend class LanguagesManager;

public:
    TranslatableLanguage();
    TranslatableLanguage(const TranslatableLanguage &other);
    virtual ~TranslatableLanguage();

    TranslatableLanguage &operator=(const TranslatableLanguage &other);
    bool operator==(const TranslatableLanguage &other) const;

    inline QString name() const { return m_name; }
    void setName(const QString &name);

    static const int TranslatableLanguageType = 1;
    inline int type() { return TranslatableLanguageType; }

    bool contextExists(const QString &context) const;

    QString message(const LanguageKey &key) const;
    QString message(const QString &context, const QString &id) const;
    QString message(const QString &context, const QString &id, const QString &optional) const;
    void setMessage(const LanguageKey &key, const QString &message);
    void setMessage(const QString &context, const QString &id, const QString &message);
    void setMessage(const QString &context, const QString &id, const QString &optional, const QString &message);
    QString translation(const LanguageKey &key) const;
    QString translation(const QString &context, const QString &id) const;
    QString translation(const QString &context, const QString &id, const QString &optional) const;
    void setTranslation(const LanguageKey &key, const QString &message);
    void setTranslation(const QString &context, const QString &id, const QString &message);
    void setTranslation(const QString &context, const QString &id, const QString &optional, const QString &message);
    bool isTranslationUpdate(const LanguageKey &key);

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

protected:
    void setTranslation(const LanguageKey &key, const TranslatableMessage &message);
    int m_symbol_id;

private:
    QString m_name;
    QHash<LanguageKey, TranslatableMessage> m_messages;
    LanguagesManager *m_manager;

};
