#include "csv.h"
#include <QFile>
#include <QTextStream>

csv::csv()
{
    clear();
    pvt_massima_dimensione_tabella=0;
    pvt_errore_massima_dimensione_tabella=false;
}

csv::csv(QChar separatore, long int massima_dimensione_tabella)
{
    clear();
    pvt_separatore=separatore;
    pvt_massima_dimensione_tabella=massima_dimensione_tabella;
    pvt_errore_massima_dimensione_tabella=false;
}

csv::csv(QChar separatore, QString nomefile, long int massima_dimensione_tabella)
{
    clear();
    pvt_separatore=separatore;
    pvt_massima_dimensione_tabella=massima_dimensione_tabella;
    pvt_errore_massima_dimensione_tabella=false;
    load(nomefile);
}

csv::csv(QTableWidget *table, bool row_header, bool column_header)
{
    //funzione che popola la struttura dati partendo da una qtablewidget
    clear();
    pvt_massima_dimensione_tabella=0;
    pvt_errore_massima_dimensione_tabella=false;
    setTable(table,row_header,column_header);
}

csv::csv(QChar separatore, QTableWidget *table, bool row_header, bool column_header)
{
    //funzione che popola la struttura dati partendo da una qtablewidget
    clear();
    pvt_massima_dimensione_tabella=0;
    pvt_errore_massima_dimensione_tabella=false;
    pvt_separatore=separatore;
    setTable(table,row_header,column_header);
}

csv::csv(QChar separatore, QString nomefile, QTableWidget *table, bool row_header, bool column_header, long int massima_dimensione_tabella)
{
    //funzione che popola una qtablewidget partendo da la struttura dati caricata da file
    clear();
    pvt_separatore=separatore;
    pvt_massima_dimensione_tabella=massima_dimensione_tabella;
    pvt_errore_massima_dimensione_tabella=false;
    load(nomefile);
    getTable(table,row_header,column_header);
}

csv::~csv()
{
    clear();
}

void csv::clear()
{
    pvt_list.clear();
    pvt_separatore=QChar();
}

void csv::addItem(unsigned int riga, unsigned int colonna, QString item)
{
    //Verifico se dvo aggiungere delle righe
    unsigned int n=rowCount();
    if(riga>=n)
    {
        for(unsigned int i=0; i<=(riga-n); i++)
        {
            //Creo una riga con il numero corrente di colonne
            QStringList line;
            for(unsigned int j=0; j<columnCount(); j++)
            {
                line << QString();
            }

            pvt_list << line;
        }
    }

    n=columnCount();
    if(colonna>=n)
    {
        for(unsigned int indice_riga=0; indice_riga<rowCount(); indice_riga++)
        {
            for(unsigned int i=0; i<=(colonna-n); i++)
            {
                pvt_list[indice_riga] << QString();
            }
        }
    }

    pvt_list[riga][colonna]=item;
}

QString csv::item(unsigned int riga, unsigned int colonna, bool *ok)
{
    if(riga<rowCount() && colonna<columnCount())
    {
        if(ok)
        {
            (*ok)=true;
        }

        return pvt_list[riga][colonna];
    }

    if(ok)
    {
        (*ok)=false;
    }

    return "";
}

QStringList csv::row(unsigned int index, bool *ok)
{
    if(index<rowCount())
    {
        if(ok)
        {
            (*ok)=true;
        }

        return pvt_list[index];
    }

    if(ok)
    {
        (*ok)=false;
    }

    return QStringList();
}

QStringList csv::column(unsigned int index, bool *ok)
{
    if(index<columnCount())
    {
        if(ok)
        {
            (*ok)=true;
        }

        QStringList list;
        for(unsigned int i=0; i<rowCount(); i++)
        {
            list << item(i, index);
        }

        return list;
    }

    if(ok)
    {
        (*ok)=false;
    }

    return QStringList();
}

unsigned int csv::rowCount()
{
    return pvt_list.size();
}

unsigned int csv::columnCount()
{
    if(pvt_list.size()>0)
    {
        return pvt_list[0].size();
    }

    return 0;
}

bool csv::save(QString nomefile)
{
    if(nomefile.isEmpty())
    {
        return false;
    }

    //Cancello un eventuale vecchio file
    QFile f(nomefile);
    if(f.exists())
    {
        f.remove();
    }

    if(!f.open(QIODevice::ReadWrite))
    {
        //Errore
        return false;
    }

    for(unsigned int indice_riga=0; indice_riga<rowCount(); indice_riga++)
    {
        QByteArray line;

        line.clear();
        for(unsigned int indice_colonna=0; indice_colonna<columnCount(); indice_colonna++)
        {
            QString str=pvt_list[indice_riga][indice_colonna];

            //Verifico la presenza di casi particolari

            //Verifico se la stringa in input contiene delle virgolette
            if(str.contains(QChar('"')))
            {
                //Ogni occorrenza delle virgolette deve essere raddoppiata
                QString tmp;
                tmp.clear();
                for(int i=0; i<str.length(); i++)
                {
                    if(str[i]==QChar('"'))
                    {
                        //Raddoppio il carattere
                        tmp.append(str[i]);
                    }
                    tmp.append(str[i]);
                }
                str=tmp;
            }

            //Verifico se la stringa necessita delle virgolette di contenimento
            if(str.contains(pvt_separatore) || str.contains(QChar('\r')) || str.contains(QChar('\n')) ||
               str.startsWith(QChar(' ')) || str.endsWith(QChar(' ')) || str.contains(QChar('"')))
            {
                str.prepend(QChar('"'));
                str.append(QChar('"'));
            }

            //Aggiungo il campo
            line.append(str);
            if(indice_colonna<(columnCount()-1))
            {
                line.append(pvt_separatore);
            }
        }

        f.write(line);
        f.write(QByteArray(1,'\r'));
        f.write(QByteArray(1,'\n'));
    }

    f.close();

    return true;
}

bool csv::load(QString nomefile)
{
    if(nomefile.isEmpty())
    {
        return false;
    }

    QFile f(nomefile);
    if(!f.exists())
    {
        //File non esistente
        return false;
    }

    if(!f.open(QIODevice::ReadWrite))
    {
        //Errore
        return false;
    }

    QTextStream in(&f);
    in.setCodec("UTF-8"); // change the file codec to UTF-8.


    pvt_errore_massima_dimensione_tabella=false;

    bool stato_interno=false;
    QString item;

    QString str/* = in.readAll()*/;
    unsigned int indice_riga=0;
    unsigned int indice_colonna=0;
    QString array = in.readAll();
    int n=array.size();
    for(int i=0; i<n; i++)
    {
        bool fine_campo=false;
        bool fine_linea=false;

        if(stato_interno)
        {
            //Sono all'interno di un campo delimitato da virgolette .. devo solo verificare l'uscita da questo stato o la presenza
            //di un doppio carattere virgolette
            if(array[i]=='"')
            {
                if(i<(n-1))
                {
                    if(array[i+1]=='"')
                    {
                        //Carattere virgolette da inserire .. salto poi le successive virgolette
                        str.append(QChar(array[i]));
                        i++;
                    }
                    else
                    {
                        //Esco dallo stato interno e chiudo il campo corrente
                        stato_interno=false;
                        fine_campo=true;
                    }
                }
                else
                {
                    //Fine del file .. esco dallo stato interno e chiudo il campo corrente
                    stato_interno=false;
                    fine_campo=true;
                    fine_linea=true;
                }
            }
            else
            {
                //Carattere da inserire nella stringa
                str.append(QChar(array[i]));
            }
        }
        else
        {
            //Sono all'esterno di un campo delimitato da virgolette .. verifico se e' presente il separatore, se invece devo
            //entrare nello stato interno o se e' presente un fine linea
            if(array[i]=='"')
            {
                stato_interno=true;
            }
            else if(QChar(array[i])==pvt_separatore)
            {
                //Campo completato
                if(i>0)
                {
                    if(array[i-1]!='"')
                    {
                        //Campo gia'  completato
                        fine_campo=true;
                    }
                }
            }
            else if(array[i]=='\r' || array[i]=='\n')
            {
                //Fine linea .. verifico se devo eliminare anche il carattere successivo
                fine_linea=true;

                //Verifico se devo anche chiudere un campo
                if(i>0)
                {
                    if(array[i-1]!='"')
                    {
                        fine_campo=true;
                    }
                }

                if(i<(n-1))
                {
                    if(array[i+1]=='\r' || array[i+1]=='\n')
                    {
                        //Non considero il campo successivo
                        i++;
                    }
                }
            }
            else
            {
                //Carattere da inserire nella stringa
                str.append(QChar(array[i]));
            }
        }

        if(fine_campo)
        {
            //Salvo il campo corrente
            addItem(indice_riga, indice_colonna, str);
            indice_colonna++;
            str.clear();
        }

        if(fine_linea)
        {
            //Passo alla linea successiva
            indice_riga++;
            indice_colonna=0;
        }

        if(pvt_massima_dimensione_tabella>0 && ((long int)(rowCount()*columnCount()))>pvt_massima_dimensione_tabella)
        {
            //Errore sulla dimensione massima della tabella
            pvt_errore_massima_dimensione_tabella=true;
            clear();

            f.close();

            return false;
        }
    }

    f.close();

    return true;
}

QChar csv::separatore()
{
    return pvt_separatore;
}

void csv::setSeparatore(QChar c)
{
    pvt_separatore=c;
}

long int csv::massimaDimensioneTabella()
{
    return pvt_massima_dimensione_tabella;
}

void csv::setMassimaDimensioneTabella(long int l)
{
    pvt_massima_dimensione_tabella=l;
}

bool csv::isErroreMassimaDimensioneTabella()
{
    return pvt_errore_massima_dimensione_tabella;
}

bool csv::getTable(QTableWidget *table, bool row_header, bool column_header, QProgressDialog *progress)
{
    if(!table)
    {
        return false;
    }

    if(progress)
    {
        progress->show();
    }

    //disabilitazione dell'update della tabella
    table->setUpdatesEnabled(false);

    //popolo la tabella
    table->clear();

    if(row_header)
    {
        for(unsigned int i=0;i<rowCount();i++)
        {
            table->setVerticalHeaderItem(i,new QTableWidgetItem(item(i,0)));
        }
    }

    if(column_header)
    {
        for(unsigned int i=0;i<columnCount();i++)
        {
            table->setHorizontalHeaderItem(i,new QTableWidgetItem(item(0,i)));
        }
    }

    table->setRowCount((row_header ? rowCount()-1 : rowCount()));
    table->setColumnCount((column_header ? columnCount()-1 : columnCount()));

    for(unsigned int i=(column_header ? 1 : 0);i<rowCount();i++)
    {
        for(unsigned int j=(row_header ? 1 : 0);j<columnCount();j++)
        {
            table->setItem((column_header ? i-1 : i),(row_header ? j-1 : j),new QTableWidgetItem(item(i,j)));
        }
        if(progress && (i%50)==0)
        {
            progress->setValue(i*columnCount());
        }
    }

    if(progress)
    {
        progress->close();
    }

    table->setUpdatesEnabled(true);

    return true;
}

bool csv::setTable(QTableWidget *table, bool row_header, bool column_header)
{
    if(!table)
    {
        return false;
    }

    //prendo i dati dalla tabella
    pvt_list.clear();

    if(row_header)
    {
        int offset=0;
        for(int i=0;i<table->rowCount();i++)
        {
            if(!table->isRowHidden(i) && table->verticalHeaderItem(i))
            {
                addItem(i-offset,0,table->verticalHeaderItem(i)->text());
            }

            if(table->isRowHidden(i))
            {
                offset++;
            }
        }
    }

    if(column_header)
    {
        int offset=0;
        for(int i=0;i<table->columnCount();i++)
        {
            if(!table->isColumnHidden(i) && table->horizontalHeaderItem(i))
            {
                addItem(0,i-offset,table->horizontalHeaderItem(i)->text());
            }

            if(table->isColumnHidden(i))
            {
                offset++;
            }
        }
    }

    int offset_row=0;
    int offset_column=0;
    for(int i=0;i<table->rowCount();i++)
    {
        if(!table->isRowHidden(i))
        {
            offset_column=0;
            for(int j=0;j<table->columnCount();j++)
            {
                if(!table->isColumnHidden(j))
                {
                    if(table->item(i,j))
                    {
                        addItem((column_header ? i+1 : i)-offset_row,(row_header ? j+1 : j)-offset_column,table->item(i,j)->text());
                    }
                }
                else
                {
                    offset_column++;
                }
            }
        }
        else
        {
            offset_row++;
        }
    }

    return true;
}
