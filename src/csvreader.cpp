#include "csvreader.h"

CSVreader::CSVreader()
{
    csv::setSeparatore(';');
}

CSVreader::CSVreader(std::string filename) :
    csv(';', QString::fromStdString(filename))
{}

std::vector<Key> CSVreader::collectKeys()
{
    std::vector<std::string> stringKeys = collectColumnAt(0);
    std::vector<Key> keys;
    std::transform(stringKeys.begin(), stringKeys.end(), std::back_inserter(keys),
                   [] (const std::string &key)
    {
        return Key(key);
    });
    return keys;
}

std::vector<std::string> CSVreader::collectColumnAt(std::size_t i)
{
    auto tmp =  csv::column(i);
    std::vector<std::string> v;
    std::transform(tmp.begin() + 2, tmp.end(), std::back_inserter(v),
                   [] (const QString& var)
    {
        return var.toStdString();
    });
    return v;
}

std::vector<std::string> CSVreader::collectIntestations()
{
    auto tmp = csv::row(1);
    std::vector<std::string> v;
    std::transform(tmp.begin() + 1, tmp.end(), std::back_inserter(v),
                   [] (const QString& var)
    {
        return var.toStdString();
    });
    return v;
}

