#include "filereader.h"

FileReader::FileReader(std::string filename)
{
    if (filename.find("csv") != std::string::npos) {
        csvReader = new csv(';', QString::fromStdString(filename));
        xlsxReader = nullptr;
    }
    else {
        xlsxReader = new xlnt::workbook();
        xlsxReader->load(filename);
        csvReader = nullptr;
    }
}

std::vector<Key> FileReader::collectKeys()
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

std::vector<std::string> FileReader::collectColumnAt(std::size_t i)
{
    std::vector<std::string> v;
    if(csvReader) {
        auto tmp =  csvReader->column((int)i);
        if (!tmp.empty())
            std::transform(tmp.begin() + 2, tmp.end(), std::back_inserter(v),
                           [] (const QString& var)
            {
                return var.toStdString();
            });
    } else {
        auto ws = xlsxReader->active_sheet();
        auto firstColIterator = ws.columns(false).begin();
        std::advance(firstColIterator, (int)i);
        auto col = *firstColIterator;

        auto colsStart = col.begin();
        std::advance(colsStart, 2);
        std::transform(colsStart, col.end(), std::back_inserter(v),
                       [] (const xlnt::cell &var)
        {
            return var.to_string();
        });
    }
    return v;
}

std::vector<std::string> FileReader::collectIntestations()
{
    std::vector<std::string> v;
    if (csvReader) {
        auto tmp = csvReader->row(1);
        if (!tmp.empty())
            std::transform(tmp.begin() + 1, tmp.end(), std::back_inserter(v),
                           [] (const QString& var)
            {
                return var.toStdString();
            });
    } else {
        auto ws = xlsxReader->active_sheet();
        auto firstRowIterator = ws.rows(false).begin();
        std::advance(firstRowIterator, 1);
        auto row = *firstRowIterator;
        auto rowStart = row.begin();
        std::advance(rowStart, 1);
        std::transform(rowStart, row.end(), std::back_inserter(v),
                       [] (const xlnt::cell &var)
        {
            return var.to_string();
        });
    }
    return v;
}

