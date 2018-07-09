#include "language.h"
#include <iterator>

std::vector<Key> Language::keys = std::vector<Key>();

Language::Language() :
    messages()
{}

Language::Language(const Language &other)
{
    *this = other;
}

Language::Language(const std::string &name, const std::vector<std::string> &strings)
{
    if (strings.size() != keys.size()) {
        std::cerr<<"Impossible to initialize language";
        return;
    }
    this->name = name;
    int i = 0;
    for(auto &key : keys) {
        messages.insert(std::make_pair(std::ref(key), strings[i]));
        i++;
    }
}

const std::string &Language::getName() const
{
    return name;
}

QList<QStandardItem *> Language::getMessagesByContext(std::string context)
{
    QList<QStandardItem *> sameCtxMessages;
    for (auto i : messages) {
        if (context.empty() || i.first.belongsTo(context)) {
           auto *item = new messageItem(i.second, name, i.first);
           sameCtxMessages.push_back(item);
        }
    }
    return sameCtxMessages;
}

const std::vector<std::string> Language::getMessages() const
{
    std::vector<std::string>tmp;
    for (auto key : keys) {
        tmp.push_back(messages.at(key));
    }
    return tmp;
}

void Language::changeMessage(const std::string &text, const Key &key)
{
    messages.at(key) = text;
}

void Language::setKeys(const std::vector<Key> &keys)
{
    Language::keys = keys;
}

const std::vector<Key> &Language::getKeys()
{
    return keys;
}
