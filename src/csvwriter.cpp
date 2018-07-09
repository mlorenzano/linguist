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

void CSVwriter::addLanguage(const Language &language)
{
    csv::addItem(1, languagesCount+1, QString::fromStdString(language.getName()));
    int i = 2;
    for (auto message : language.getMessages()) {
        csv::addItem(i, languagesCount+1, QString::fromStdString(message));
        i++;
    }
    ++languagesCount;
}
