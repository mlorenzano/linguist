#ifndef INCLUDELANGUAGE_H
#define INCLUDELANGUAGE_H

#include"key.h"
#include <unordered_map>
#include <list>
#include <string>
#include <iostream>

class Language
{
public:
    Language();
    Language(const Language &other);
    void addItem(const Key &key, std::string message);
    std::list<std::string> getMessagesByContext(std::string context);
private:
    std::unordered_map<std::string, Key> messages;
};

#endif // INCLUDELANGUAGE_H

