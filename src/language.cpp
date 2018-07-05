#include "language.h"
#include <iterator>

Language::Language() :
    messages()
{}

Language::Language(const Language &other)
{
    *this = other;
}

void Language::addItem(const Key &key, std::string message)
{
    messages.insert(std::make_pair(message, key));
}

std::list<std::string> Language::getMessagesByContext(std::string context)
{
    std::list<std::string> sameCtxMessages;
    for (auto  i : messages) {
        if (i.second.belongsTo(context)) {
           sameCtxMessages.push_back(i.first);
        }
    }
    return sameCtxMessages;
}
