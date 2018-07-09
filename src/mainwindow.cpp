#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "csvreader.h"
#include "csvwriter.h"
#include "language.h"

#include <iostream>

#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    supportedType{tr("Comma Separated Values (*.csv)")},
    tableManager()
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
    CSVwriter writer;
    QString destFilename = QFileDialog::getSaveFileName(this, tr("Import languages"),  QString(),supportedType); //TODO: aggiungerlo
    if (destFilename.isEmpty())
        return;
    writer.setKeys(Language::getKeys());
    for (auto i : tableManager.getLanguages()) {
        writer.addLanguage(i);
    }
    writer.save(destFilename);
}

void MainWindow::on_actionImport_triggered()
{
    QString destFilename = QFileDialog::getOpenFileName(this, tr("Import languages"), "",supportedType);
    if (destFilename.isEmpty())
        return;
    tableManager.clear();

    CSVreader reader(destFilename.toStdString());

    Language::setKeys(reader.collectKeys());
    std::vector<std::string> intestations = reader.collectIntestations();
    for (int i = 0; i < intestations.size(); i++) {
        tableManager.insertLanguage(intestations[i],
                                    Language(intestations[i], reader.collectColumnAt(i+1)));
    }

    ui->languageTable->setModel(tableManager.getTableByContext());
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











