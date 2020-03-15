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

    std::string toString() const;
    bool fromString(std::string keyString);
    bool belongsTo(const std::string &context, const std::string pageOfContext) const;

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

struct KeyHasher
{
    std::size_t operator ()(const Key& k) const
    {
        using std::size_t;
        using std::hash;
        using std::string;

        return ((hash<string>()(k.getContext())
                 ^ (hash<string>()(k.getId()) << 1)) >> 1)
                ^ (hash<string>()(k.getPageOfContext()) << 1);
    }
};

#endif
