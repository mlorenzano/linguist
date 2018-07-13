#ifndef CSV_H
#define CSV_H

#include "csv_global.h"

#include <csv_private.h>

class CSVSHARED_EXPORT Csv
{
public:
    Csv();
    ~Csv();

    //Salvataggio
    bool load(const std::string& filename, char separator);

    //Caricamento
    bool save(const std::string& filename, char separator);

    //Pulizia
    void clear();

    //Informazioni sullo stato corrente
    size_t rowCount();
    size_t columnCount();

    //Popolazione
    void setItem(const size_t row, const size_t column, const std::string& value);

    //Lettura
    std::string item(const size_t row, const size_t column);
    std::vector<std::string> row(const size_t index);
    std::vector<std::string> column(const size_t index);

private:
    Csv_private *worker;
};

#endif // CSV_H
