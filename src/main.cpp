#include "mainwindow.h"
#include <QApplication>

#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    qApp->setStyle(QStyleFactory::create("Fusion")); 
    
    return a.exec();
}
