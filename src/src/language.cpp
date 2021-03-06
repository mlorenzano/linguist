#include "language.h"
#include <iterator>

std::vector<Key> Language::keys = std::vector<Key>();

Language::Language()
    : messages()
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
    for (auto &key : keys) {
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

QList<QStandardItem *> Language::getMessagesByContext(const std::string &context,
                                                      const std::string &page)
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
    std::vector<std::string> tmp;
    for (auto key : keys) {
        tmp.push_back(messages.at(key));
    }
    return tmp;
}

const std::string &Language::messageAt(const Key &key) const
{
    return messages.at(key);
}

bool Language::changeMessage(const std::string &oldText, const std::string &text)
{
    auto iterator = std::find_if(messages.begin(), messages.end(),
                                 [&](const auto &it){ return it.second == oldText;});

    if (iterator == messages.end()) {
        return false;
    }

    iterator->second = text;

    return true;
}

void Language::setKeys(const std::vector<Key> &keys)
{
    Language::keys = keys;
}

const std::vector<Key> &Language::getKeys()
{
    return keys;
}

void Language::replaceMessage(const std::string &oldMsg, const std::string &newMsg,
                              int caseSensitivity)
{
    for (const auto &key : keys) {
        // Fuck off I'm using QStrings...
        auto msgAsQString = QString::fromStdString(messages.at(key));
        msgAsQString.replace(oldMsg.c_str(), newMsg.c_str(),
                             caseSensitivity ? Qt::CaseSensitive : Qt::CaseInsensitive);
        messages.at(key) = msgAsQString.toStdString();
    }
}
