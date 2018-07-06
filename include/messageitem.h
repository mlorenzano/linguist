#ifndef INCLUDEMESSAGEITEM_H
#define INCLUDEMESSAGEITEM_H

#include "key.h"
#include <QStandardItem>
#include <iostream>

class messageItem : public QStandardItem
{
public:
    messageItem();
    messageItem(const std::string &text, const std::string &language, const Key &key);

    std::string getLanguage() const;
    Key getKey() const;

private:
    std::string language;
    Key key;
};

#endif // INCLUDEMESSAGEITEM_H
