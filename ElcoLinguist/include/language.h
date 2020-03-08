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
    void changeMessage(const std::string &text, const Key &key);

    static void setKeys(const std::vector<Key> &keys);
    static const std::vector<Key> &getKeys();

private:
    std::string name;
    static std::vector<Key> keys;
    std::unordered_map<Key, std::string, KeyHasher> messages;
};

#endif // INCLUDELANGUAGE_H
