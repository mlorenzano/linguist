#ifndef INCLUDELANGUAGE_H
#define INCLUDELANGUAGE_H

#include"key.h"

#include <QList>
#include <QStandardItem>

#include <unordered_map>
#include <list>
#include <string>
#include <iostream>

struct KeyHasher
{
    std::size_t operator ()(const Key& k) const
    {
        using std::size_t;
        using std::hash;
        using std::string;

        return ((hash<string>()(k.getContext())
                 ^ (hash<string>()(k.getId()) << 1)) >> 1)
                ^ (hash<string>()(k.getPageOfContext()) << 1);
    }
};

class Language
{
public:
    Language();
    Language(const Language &other);
    //Language(std::list<std::string> &&strings);
    void addItem(const Key &key, std::string message);
    QList<QStandardItem *> getMessagesByContext(std::string context = "");
    static void setKeys(const std::list<std::string> &keys);
private:
    static std::list<std::string> keys;
    std::unordered_map<Key, std::string, KeyHasher> messages;
};

#endif // INCLUDELANGUAGE_H

