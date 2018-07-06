#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "csv.h"

#include <language.h>
#include <algorithm>

#include <iostream>

#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    supportedType{tr("Comma Separated Values (*.csv)")},
    tableManager(),
    csvReader(QChar(';'), nullptr, true, false)
{
    ui->setupUi(this);
    createToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createToolBar()
{
    QAction *addLanguageButton = new QAction();
    addLanguageButton->setIcon(QIcon(":/icons/icons/Gnome-List-Add-64.png")); //plus symbol
    connect(addLanguageButton, SIGNAL(triggered(bool)), this, SLOT(on_actionAdd_Language_triggered()));
    addLanguageButton->setToolTip("Add Language");
    ui->topToolBar->addAction(addLanguageButton);
    
    QAction *removeLanguageButton = new QAction();
    removeLanguageButton->setIcon(QIcon(":/icons/icons/Gnome-List-Remove-64.png")); //minus symbol
    connect(removeLanguageButton, SIGNAL(triggered(bool)), this, SLOT(on_actionRemove_Language_triggered()));
    removeLanguageButton->setToolTip("Remove Language");
    ui->topToolBar->addAction(removeLanguageButton);
    
    ui->topToolBar->addSeparator();
    
    QAction *importLanguagesbutton = new QAction();
    importLanguagesbutton->setIcon(QIcon(":/icons/icons/gnome_import.png")); //import symbol
    connect(importLanguagesbutton, SIGNAL(triggered(bool)), this, SLOT(on_actionImport_triggered()));
    importLanguagesbutton->setToolTip("Import");
    ui->topToolBar->addAction(importLanguagesbutton);
    
    QAction *exportLanguagesButton = new QAction();
    exportLanguagesButton->setIcon(QIcon(":/icons/icons/gnome_export.png")); //import symbol
    connect(exportLanguagesButton, SIGNAL(triggered(bool)), this, SLOT(on_actionExport_triggered()));
    exportLanguagesButton->setToolTip("Export");
    ui->topToolBar->addAction(exportLanguagesButton);
    
    ui->topToolBar->addSeparator();
    
    QAction *settingsButton = new QAction();
    settingsButton->setIcon(QIcon(":/icons/icons/Gnome-System-Run-64.png")); //gear symbol
    connect(settingsButton, SIGNAL(triggered(bool)), this, SLOT(on_actionPreferences_triggered()));
    settingsButton->setToolTip("Preferences");
    ui->topToolBar->addAction(settingsButton);
}

void MainWindow::on_actionExport_triggered()
{

}

void MainWindow::on_actionImport_triggered()
{
    QString destFilename = QFileDialog::getOpenFileName(this, tr("Import languages"),  QString(),supportedType); //TODO: aggiungerlo
    if (destFilename.isEmpty())
        return;
    tableManager.clear();
    csvReader.load(destFilename);
    populateTable();
    ui->languageTable->setModel(tableManager.getTableByContext());
    ui->languageTable->resizeColumnsToContents();
}

void MainWindow::on_actionPreferences_triggered()
{
    settingsDialog *d = new settingsDialog();
    d->show();
    d->exec();
}

void MainWindow::on_actionAdd_Language_triggered()
{
    
}

void MainWindow::on_actionRemove_Language_triggered()
{
    
}

void MainWindow::on_actionFilters_triggered()
{
    
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *d{new AboutDialog(this)};
    d->exec();
}

std::vector<std::string> MainWindow::collectIntestations()
{
    auto tmp =  csvReader.row(1);
    std::vector<std::string> v;
    std::transform(tmp.begin() + 1, tmp.end(), std::back_inserter(v), [] (const QString& var)
    { return var.toStdString();});

    return v;
}

std::vector<std::string> MainWindow::collectColumnAt(std::size_t i)
{
    auto tmp =  csvReader.column(i);
    std::vector<std::string> v;
    std::transform(tmp.begin() + 2, tmp.end(), std::back_inserter(v), [] (const QString& var)
    { return var.toStdString();});

    return v;
}

void MainWindow::populateTable()
{
    std::vector<std::string> intestations = collectIntestations();
    Language::setKeys(collectColumnAt(0));
    for (int i = 0; i < intestations.size(); i++) {
        tableManager.insertLanguage(intestations[i], Language(intestations[i], collectColumnAt(i+1)));
    }
}











