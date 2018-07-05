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

Language::Language(std::vector<std::string> &strings)
{
    if (strings.size() != keys.size()) {
        std::cerr<<"Impsssible to initialize language";
        return;
    }
//    for(auto key : keys) {
//        messages.insert(std::make_pair(key, strings.front()));
//        strings.pop_front();
//    }
}

void Language::addItem(const Key &key, std::string message)
{
    //messages.insert(std::pair<Key &, std::string> (std::cref(key), message));
}

QList<QStandardItem *> Language::getMessagesByContext(std::string context)
{
    QList<QStandardItem *> sameCtxMessages;
    for (auto  i : messages) {
//        if (context.empty() || i.first.belongsTo(context)) {
//           auto *item = new QStandardItem(QString::fromStdString(i.second));
//           sameCtxMessages.push_back(item);
//        }
    }
    return sameCtxMessages;
}

void Language::setKeys(const std::vector<std::string> &keys)
{
    Language::keys = keys;
}
