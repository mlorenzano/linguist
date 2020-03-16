#ifndef INCLUDELANGUAGE_H
#define INCLUDELANGUAGE_H

#include "key.h"
#include "messageitem.h"

#include <QList>

#include <iostream>
#include <string>
#include <unordered_map>

class Language
{
public:
    Language();
    Language(const std::string &name);
    Language(const std::string &name, const std::vector<std::string> &strings);
    Language(const Language &other);

    const std::string &getName() const;
    QList<QStandardItem *> getMessagesByContext(const std::string &context = "",
                                                const std::string &page = "");
    const std::vector<std::string> getMessages() const;

    static void setKeys(const std::vector<Key> &keys);
    static const std::vector<Key> &getKeys();

    void replaceMessage(const std::string &oldMsg, const std::string &newMsg);

private:
    std::string name;
    static std::vector<Key> keys;
    std::unordered_map<Key, std::string, KeyHasher> messages;
};

#endif // INCLUDELANGUAGE_H
