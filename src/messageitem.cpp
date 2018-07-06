#include "messageitem.h"

messageItem::messageItem() : QStandardItem(),
    language(),
    key()
{}

messageItem::messageItem(const std::string &text, const std::string &language, const Key &key) :
    QStandardItem(QString::fromStdString(text))
{
    this->language = language;
    this->key = std::cref(key);
}

std::string messageItem::getLanguage() const
{
    return language;
}

Key messageItem::getKey() const
{
    return key;
}


