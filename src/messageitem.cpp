#include "messageitem.h"

messageItem::messageItem() : QStandardItem(),
    language(),
    key()
{}

messageItem::messageItem(const std::string &text, const std::string &language, const Key &key) :
    QStandardItem(QString::fromStdString(text).replace("\\n", "\n"))
{
    changeColor();
    this->language = language;
    this->key = std::cref(key);
}

void messageItem::changeColor()
{
    if (QStandardItem::text() == "")
        QStandardItem::setBackground(QColor(253, 177, 94));
}
std::string messageItem::getLanguage() const
{
    return language;
}

Key messageItem::getKey() const
{
    return key;
}
