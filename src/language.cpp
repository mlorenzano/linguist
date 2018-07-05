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

QList<QStandardItem *> Language::getMessagesByContext(std::string context)
{
    QList<QStandardItem *> sameCtxMessages;
    for (auto  i : messages) {
        if (context.empty() || i.second.belongsTo(context)) {
           auto *item = new QStandardItem(QString::fromStdString(i.first));
           sameCtxMessages.push_back(item);
        }
    }
    return sameCtxMessages;
}
