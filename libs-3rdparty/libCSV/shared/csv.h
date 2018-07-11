#ifndef CSV_H
#define CSV_H

#include "csv_global.h"
#include <QStringList>
#include <QList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QProgressDialog>

class CSV_EXPORT csv
{
public:
    csv();
    csv(QChar separatore, long int massima_dimensione_tabella=0);
    csv(QChar separatore, QString filename, long int massima_dimensione_tabella=0);
    csv(QTableWidget *table, bool row_header=false, bool column_header=false);
    csv(QChar separatore, QTableWidget *table, bool row_header=false, bool column_header=false);
    csv(QChar separatore, QString filename, QTableWidget *table, bool row_header=false, bool column_header=false, long int massima_dimensione_tabella=0);
    ~csv();

    void clear();

    //Informazioni sullo stato corrente
    unsigned int rowCount();
    unsigned int columnCount();

    //Popolazione della classe
    void addItem(unsigned int row, unsigned int column, QString item);

    //Lettura della classe
    QString item(unsigned int row, unsigned int column, bool *ok=0);
    QStringList row(unsigned int index, bool *ok=0);
    QStringList column(unsigned int index, bool *ok=0);

    //Salvataggio su file
    bool save(QString filename);

    //Caricamento da file
    bool load(QString filename);

    //Accesso al separatore
    QChar separatore();
    void setSeparatore(QChar c);

    long int massimaDimensioneTabella();
    void setMassimaDimensioneTabella(long int l);

    bool isErroreMassimaDimensioneTabella();

    //Gestione tabella
    bool getTable(QTableWidget *table, bool row_header=false, bool column_header=false, QProgressDialog *progress=0x00000000);
    bool setTable(QTableWidget *table, bool row_header=false, bool column_header=false);

private:
    QList<QStringList>  pvt_list;
    QChar               pvt_separatore;
    long int            pvt_massima_dimensione_tabella;
    bool                pvt_errore_massima_dimensione_tabella;
};

#endif // CSV_H
