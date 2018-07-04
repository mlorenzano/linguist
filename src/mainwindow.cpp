#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "languagesexporter.h"
#include "languagesmanager.h"
#include "languagesmanagermodel.h"
#include "csvlanguagesexporter.h"
#include "customitemdelegate.h"

#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>

const QString settingsGroup = "LanguageManager";
const QString lastDirectory = "lastDirectoryUsed";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_languagesTable(new LanguagesManagerModel()),
    m_manager(LanguagesManager::instance()),
    supportedType{tr("Comma Separated Values (*.csv)")}
{
    ui->setupUi(this);

    ui->languageTable->setItemDelegate(new CustomItemDelegate);
    ui->languageTable->setModel(m_languagesTable);
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
    QSettings settings;
    settings.beginGroup(settingsGroup);
    QString lastDir = settings.value(lastDirectory, QString()).toString();
    settings.endGroup();

    QString destFilename = QFileDialog::getOpenFileName(this,
                                                        tr("Import languages"),
                                                        lastDir,
                                                        supportedType); //TODO: aggiungerlo

    if (destFilename.isEmpty())
        return;

    settings.beginGroup(settingsGroup);
    settings.setValue(lastDirectory, QFileInfo(destFilename).absolutePath());
    settings.endGroup();

    std::unique_ptr<AbstractLanguagesExporter> exporter{new CsvLanguagesExporter(m_manager)};
    exporter->setFilename(destFilename);

    if (exporter->importLanguages()) {
        m_languagesTable->init();
        QMessageBox::information(this, tr("Languages Manager"),
                                 tr("Languages succesfully imported"));
    }
    else
        QMessageBox::warning(this, tr("Languages Manager"),
                             tr("Error while importing languages"));
}

void MainWindow::on_actionPreferences_triggered()
{
    settingsDialog *d = new settingsDialog();
    d->show();
    //d->exec();
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
