#include <iostream>
#ifndef INCLUDEKEY_H
#define INCLUDEKEY_H

class Key
{
public:
    Key() = default;
    ~Key() = default;
    Key(const Key &other);
    Key(std::string context, std::string id, std::string pageOfContext);
    Key(std::string keyString);

    std::string toString();
    bool fromString(std::string keyString);
    bool belongsTo(std::string context);

    Key &operator =(const Key &other);
    bool operator ==(const Key &other);
    bool operator !=(const Key &other);

private:
    std::string id;
    std::string context;
    std::string pageOfContext;
};
#endif
