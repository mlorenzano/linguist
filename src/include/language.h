#pragma once

#include "Keys.hpp"
#include "messageitem.h"

class Language
{
public:
    explicit Language(const Keys &keys);

    const std::string &getName() const;
    QList<QStandardItem *> getMessagesByContext(const std::string &context = "",
                                                const std::string &page = "");
    const std::vector<std::string> getMessages() const;
    const std::string &messageAt(const Key &key) const;
    bool changeMessage(const std::string &oldText, const std::string &text);

    static void setKeys(const std::vector<Key> &keys);
    static const std::vector<Key> &getKeys();

    void replaceMessage(const std::string &oldMsg, const std::string &newMsg);

private:
    std::string name;
    const Keys &m_keys;
    std::unordered_map<Key, QString, KeyHasher> messages;
};
