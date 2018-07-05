#ifndef INCLUDELANGUAGETABLEMANAGER_H
#define INCLUDELANGUAGETABLEMANAGER_H

#include "language.h"

#include <qstandarditemmodel.h>

#include <unordered_map>
#include <list>

class languagesTableManager
{
public:
    languagesTableManager();
    void insertLanguage(std::string languageName, Language language);
    QStandardItemModel &getTableByContext(std::string context);

    /*
     * QStandardItemModel *model = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns
       &nbsp;&nbsp;&nbsp; model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
    &nbsp;&nbsp;&nbsp; model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
    &nbsp;&nbsp;&nbsp; model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));
    &nbsp;&nbsp;&nbsp; ui->tableView->setModel(model);
 */
private:
    QStandardItemModel languagesTable;
    std::unordered_map<std::string, Language> languages;
};

#endif // INCLUDELANGUAGETABLEMANAGER_H
