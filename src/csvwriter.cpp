#include "csvwriter.h"

CSVwriter::CSVwriter() :
    csv()
{
    infKeyText = "Please, do not edit the \"Key\" column. It's used to identify the message.";
    languagesCount = 0;
    csv::addItem(0, 0, QString::fromStdString(infKeyText));
    csv::setSeparatore(';');
}

void CSVwriter::setKeys(std::vector<Key> keys)
{
    csv::addItem(1, 0, "Key");
    int i = 2;
    for (auto key : keys) {
        csv::addItem(i, 0, QString::fromStdString(key.toString()));
        i++;
    }
}

void CSVwriter::addLanguages(const std::vector<Language> &languages)
{
    for (auto lang : languages) {
        csv::addItem(1, languagesCount+1, QString::fromStdString(lang.getName()));
        int i = 2;
        for (auto message : lang.getMessages()) {
            csv::addItem(i, languagesCount+1, QString::fromStdString(message));
            i++;
        }
        ++languagesCount;
    }

}
