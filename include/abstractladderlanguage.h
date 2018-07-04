#pragma once

#include <QString>
#include <QList>

class LanguagesManager;
class LanguageKey;

class AbstractLadderLanguage
{
public:
    /*!
     * \brief name
     * \return
     */
    virtual QString name() const = 0;

    /*!
     * \brief setName
     * \param name
     */
    virtual void setName(const QString &name) = 0;

    /*!
     * \brief type
     * \return
     */
    virtual int type() = 0;

    /*!
     * \brief contextExists
     * \param context
     * \return
     */
    virtual bool contextExists(const QString &context) const = 0;

    /*!
     * \brief message
     * \param key
     * \return
     */
    virtual QString message(const LanguageKey &key) const = 0;

    /*!
     * \brief message
     * \param context
     * \param id
     * \return
     */
    virtual QString message(const QString &context, const QString &id) const = 0;

    /*!
     * \brief message
     * \param context
     * \param id
     * \param optional
     * \return
     */
    virtual QString message(const QString &context, const QString &id, const QString &optional) const = 0;

    /*!
     * \brief setMessage
     * \param key
     * \param message
     * \return
     */
    virtual void setMessage(const LanguageKey &key, const QString &message) = 0;

    /*!
     * \brief setMessage
     * \param context
     * \param id
     * \param message
     * \return
     */
    virtual void setMessage(const QString &context, const QString &id, const QString &message) = 0;

    /*!
     * \brief setMessage
     * \param context
     * \param id
     * \param optional
     * \param message
     */
    virtual void setMessage(const QString &context, const QString &id, const QString &optional, const QString &message) = 0;

    /*!
     * \brief clear
     */
    virtual void clear() = 0;

    /*!
     * \brief load
     * \return
     */
    virtual bool load() = 0;

    /*!
     * \brief save
     * \return
     */
    virtual bool save() = 0;

    /*!
     * \brief manager
     * \return
     */
    virtual LanguagesManager *manager() const = 0;

    /*!
     * \brief setManager
     * \param manager
     */
    virtual void setManager(LanguagesManager *manager) = 0;

    /*!
     * \brief size
     * \return
     */
    virtual int size() = 0;

    /*!
     * \brief keys
     * \return
     */
    virtual QList<LanguageKey> keys() const = 0;

    /*!
     * \brief contexts
     * \return
     */
    virtual QStringList contexts() const = 0;

    /*!
     * \brief displayed
     * \param key
     * \return
     */
    virtual QString displayed(const LanguageKey &key) const = 0;

    /*!
     * \brief displayed
     * \param context
     * \param id
     * \return
     */
    virtual QString displayed(const QString &context, const QString &id) const = 0;

    /*!
     * \brief displayed
     * \param context
     * \param id
     * \param optional
     * \return
     */
    virtual QString displayed(const QString &context, const QString &id, const QString &optional) const = 0;

};
