#pragma once

#include "language.h"

#include <QRunnable>

class FileWriter : public QRunnable
{
public:
    explicit FileWriter(const QString &filename,
                        const QVector<Language> &languages,
                        QObject *caller);

    void run() override;

private:
    QString m_filename;
    QVector<Language> m_languages;
    QObject *m_caller;

    void save();
};
