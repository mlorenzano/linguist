#include "filewriter.h"

FileWriter::FileWriter(std::string filename)
{
    infKeyText = "Please, do not edit the \"Key\" column. It's used to identify the message.";
    languagesCount = 0;
    this->filename = filename;
    if (filename.find("csv") != std::string::npos) {
        csvWriter = new csv();
        csvWriter->addItem(0, 0, QString::fromStdString(infKeyText));
        xlsxWriter = nullptr;
    }
    else if (filename.find("xlsx") != std::string::npos ||
             filename.find("xlx") != std::string::npos){
        xlsxWriter = new xlnt::workbook();
        xlnt::worksheet ws = xlsxWriter->active_sheet();
        ws.cell("A1").value(infKeyText);
        csvWriter = nullptr;
    }
}

void FileWriter::setKeys(std::vector<Key> keys)
{
    if (csvWriter) {
        csvWriter->addItem(1, 0, "Key");
        int i = 2;
        for (auto key : keys) {
            csvWriter->addItem(i, 0, QString::fromStdString(key.toString()));
            i++;
        }
    } else if (xlsxWriter) {
        xlnt::worksheet ws = xlsxWriter->active_sheet();
        ws.rows(false)[1][0].value("Key");
        int i = 2;
        for (auto key : keys) {
            ws.rows(false)[i][0].value(key.toString());
            i++;
        }
    }
}

void FileWriter::addLanguages(const std::vector<Language> &languages)
{
    if (csvWriter) {
        for (auto lang : languages) {
            csvWriter->addItem(1, languagesCount+1, QString::fromStdString(lang.getName()));
            int i = 2;
            for (auto message : lang.getMessages()) {
                csvWriter->addItem(i, languagesCount+1, QString::fromStdString(message));
                i++;
            }
            ++languagesCount;
        }
    } else if (xlsxWriter) {
        for (auto lang : languages) {
            xlnt::worksheet ws = xlsxWriter->active_sheet();
            ws.rows(false)[1][languagesCount+1].value(lang.getName());
            int i = 2;
            for (auto message : lang.getMessages()) {
                ws.rows(false)[i][languagesCount+1].value(message);
                i++;
            }
            ++languagesCount;
        }
    }

}

void FileWriter::save()
{
    if (csvWriter)
        csvWriter->save(QString::fromStdString(filename));
    else if (xlsxWriter)
        xlsxWriter->save(filename);
}
