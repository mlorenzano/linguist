#include "languagestablemanager.h"

languagesTableManager::languagesTableManager() :
    languages()
{}

QStandardItemModel *languagesTableManager::getTableByContext(std::string context)
{
    languagesTable = new QStandardItemModel(0, languages.size());
    int i = 0;
    for (auto lang : languages) {
        languagesTable->setHorizontalHeaderItem(i, new QStandardItem(QString::fromStdString(lang.first)));
        languagesTable->appendColumn(lang.second.getMessagesByContext(context));
        ++i;
    }
    return languagesTable;
}

void languagesTableManager::insertLanguage(std::string languageName, Language language)
{
    languages.insert({languageName, language});
}
/*
 * QStandardItemModel *model = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns
   &nbsp;&nbsp;&nbsp; model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
&nbsp;&nbsp;&nbsp; model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
&nbsp;&nbsp;&nbsp; model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));
&nbsp;&nbsp;&nbsp; ui->tableView->setModel(model);
*/
