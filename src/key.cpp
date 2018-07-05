#include "key.h"
#include <string>
#include <QString>
#include <QRegExp>
#include <QStringList>

Key::Key(const Key &other)
{
    *this = other;
}

Key::Key(std::string context, std::string id, std::string pageOfContext)
{
    this->context = context;
    this->id = id;
    this->pageOfContext = pageOfContext;
}

Key::Key(std::string keyString)
{
    this->fromString(keyString);
}

std::string Key::toString()
{

    std::string keyString = "";
    keyString += "ct=\"" + context + "\" ";
    keyString += "id=\"" + id + "\" ";
    keyString += "op=\"" + pageOfContext + "\" ";
    return keyString;
}
bool Key::fromString(std::string keyString)
{
    QString key, value, param;
    QRegExp reValue("((ct|id|op)=\"+[\\S]+\"+)");
    int pos = 0;
    bool ok = false;
    QString str = QString().fromStdString(keyString);

    context.clear();
    id.clear();
    pageOfContext.clear();

    while ((pos = reValue.indexIn(str, pos)) != -1) {
        ok = true;
        param = reValue.cap(1);
        pos += reValue.matchedLength();
        key = param.split("=").first();
        value = param.split("=").last().replace("\"", "");

        if (key == "ct")
            context = value.toStdString();

        else if (key == "id")
            id = value.toStdString();

        else if (key == "op")
            pageOfContext = value.toStdString();

        else {
            context.clear();
            id.clear();
            pageOfContext.clear();
            return false;
        }
    }
    return false;
}

bool Key::belongsTo(std::string context)
{
    return this->context == context;
}
Key &Key::operator =(const Key &other)
{
    context = other.context;
    id = other.id;
    pageOfContext = other.context;
    return *this;
}

bool Key::operator ==(const Key &other)
{
    return context == other.context &&
           id == other.id &&
           pageOfContext == other.pageOfContext;
}
bool Key::operator !=(const Key &other)
{
    return !(*this == other);
}


