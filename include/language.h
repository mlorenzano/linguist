#ifndef INCLUDELANGUAGE_H
#define INCLUDELANGUAGE_H

#include"key.h"

#include <QList>
#include <QStandardItem>

#include <unordered_map>
#include <list>
#include <string>
#include <iostream>

class Language
{
public:
    Language();
    Language(const Language &other);
    Language(const std::vector<std::string> &strings);
    QList<QStandardItem *> getMessagesByContext(std::string context = "");
    static void setKeys(const std::vector<std::string> &keys);

private:
    static std::vector<std::string> keys;
    std::unordered_map<Key , std::string, KeyHasher> messages;
};

#endif // INCLUDELANGUAGE_H

