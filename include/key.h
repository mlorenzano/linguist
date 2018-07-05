#ifndef INCLUDEKEY_H
#define INCLUDEKEY_H

#include <iostream>

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
    bool operator ==(const Key &other) const;
    bool operator !=(const Key &other) const;

    std::string getContext() const;

    std::string getId() const;

    std::string getPageOfContext() const;

private:
    std::string context;
    std::string id;
    std::string pageOfContext;
};
#endif
