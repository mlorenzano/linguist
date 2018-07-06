#include "language.h"
#include <iterator>

std::vector<std::string> Language::keys = std::vector<std::string>();

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

void Language::setKeys(const std::vector<std::string> &keys)
{
    Language::keys = keys;
}
