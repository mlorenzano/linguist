#include "language.h"
#include <iterator>

std::vector<Key> Language::keys = std::vector<Key>();

Language::Language() :
    messages()
{}

Language::Language(const std::string &name)
{
    this->name = name;
    for (auto &key : keys) {
        messages.insert(std::make_pair(std::cref(key), ""));
    }
}

Language::Language(const std::string &name, const std::vector<std::string> &strings)
{
    this->name = name;
    int i = 0;
    for(auto &key : keys) {
        messages.insert(std::make_pair(std::ref(key), strings[i]));
        i++;
    }
}

Language::Language(const Language &other)
{
    *this = other;
}

const std::string &Language::getName() const
{
    return name;
}

QList<QStandardItem *> Language::getMessagesByContext(const std::string &context, const std::string &page)
{
    QList<QStandardItem *> sameCtxMessages;
    for (auto i : keys) {
        if (i.belongsTo(context, page)) {
           auto *item = new messageItem(messages.at(i), name, i);
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
