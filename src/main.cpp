#include "mainwindow.h"
#include <QApplication>

#include <QStyleFactory>

const QString VERSION = "0.3.0";

void loadApplicationData()
{
    QCoreApplication::setOrganizationDomain("www.elcoelettronica.it");
    QCoreApplication::setOrganizationName("Elco Elettronica SRL");
    QCoreApplication::setApplicationName("Elco Linguist");
    QCoreApplication::setApplicationVersion(VERSION);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loadApplicationData();

    MainWindow w;


    w.show();
    
    qApp->setStyle(QStyleFactory::create("Fusion")); 
    
    return a.exec();
}
