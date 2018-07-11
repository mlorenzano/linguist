#include "mainwindow.h"
#include <QApplication>

#include <QStyleFactory>

const QString VERSION = "0.1.0";

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
    MainWindow w;

    loadApplicationData();

    w.show();
    
    qApp->setStyle(QStyleFactory::create("Fusion")); 
    
    return a.exec();
}
