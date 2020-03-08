#pragma once

#include "language.h"

#include <QRunnable>

class FileWriter : public QRunnable
{
public:
    explicit FileWriter(const std::string &filename,
                        const std::vector<Language> &language,
                        QObject *caller);

    void run() override;

private:
    std::string m_filename;
    std::vector<Language> m_languages;
    QObject *m_caller;

    void save();
};
