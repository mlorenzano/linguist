#ifndef INCLUDELANGUAGE_H
#define INCLUDELANGUAGE_H

#include "key.h"
#include "messageitem.h"

#include <QList>

#include <unordered_map>
#include <string>
#include <iostream>

class Language
{
public:
    Language();
    Language(const Language &other);
    Language(const std::string &name, const std::vector<std::string> &strings);
    QList<QStandardItem *> getMessagesByContext(std::string context = "");
    static void setKeys(const std::vector<std::string> &keys);

private:
    std::string name;
    static std::vector<std::string> keys;
    std::unordered_map<Key , std::string, KeyHasher> messages;
};

#endif // INCLUDELANGUAGE_H

