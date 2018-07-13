#ifndef CSV_PRIVATE_H
#define CSV_PRIVATE_H

#include <string>
#include <vector>

class Csv_private
{

public:
    Csv_private();

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
    std::vector<std::vector<std::string> > m_items;
    size_t m_columnCount;

    //Lettura del file
    std::vector<std::string> readFile(const std::string& path);

    //Funzione di split
    void split(const std::string& s, const char delim, std::vector<std::string> *out);
};

#endif // CSV_PRIVATE_H
