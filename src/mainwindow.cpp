#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "csvreader.h"
#include "csvwriter.h"
#include "language.h"
#include "exportlanguagesdialog.h"

#include <iostream>

#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    supportedType{tr("Comma Separated Values (*.csv)")},
    tableManager(),
    sortFilter(new QSortFilterProxyModel()),
    searchLine(new QLineEdit())
{
    ui->setupUi(this);
    createToolBar();

    connect(searchLine, &QLineEdit::textEdited, this, &MainWindow::searchString);
    currentContext = "";
    sortFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    sortFilter->setSourceModel(tableManager.getTableByContext());
    ui->languageTable->setModel(sortFilter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExport_triggered()
{
    CSVwriter writer;
    QString destFilename = QFileDialog::getSaveFileName(this, tr("Import languages"),  workingDirectory,supportedType); //TODO: aggiungerlo
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
    std::string destFilename = QFileDialog::getOpenFileName(this, tr("Import languages"), workingDirectory,supportedType).toStdString();
    if (destFilename.empty())
        return;
    tableManager.clear();

    QFileInfo info(QString::fromStdString(destFilename));
    workingDirectory = info.path();
    QWidget::setWindowTitle(info.fileName() + " - " + QWidget::windowTitle());

    CSVreader reader(destFilename);
    Language::setKeys(reader.collectKeys());
    std::vector<std::string> intestations = reader.collectIntestations();
    tableManager.setDefault(Language(intestations[0], reader.collectColumnAt(1)));
    for (int i = 1; i < intestations.size(); i++) {
        tableManager.insertLanguage(intestations[i],
                                    Language(intestations[i], reader.collectColumnAt(i+1)));
    }

    currentContext = "";
    populateContextTree();
    updateLanguageTable();
}

void MainWindow::on_actionPreferences_triggered()
{
    settingsDialog *d = new settingsDialog();
    d->show();
    d->exec();
}

void MainWindow::on_actionAdd_Language_triggered()
{
    std::string name = QInputDialog::getText(this, tr("New language"),
                                             tr("Insert language name:")).toStdString();
    if (name.empty())
        return;
    if (!tableManager.insertLanguage(name, Language(name)))
        QMessageBox::information(this, tr("Error"),
                                 tr("This language already exists."));
    updateLanguageTable();
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

    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    ui->topToolBar->addWidget(empty);

    auto lblSearch = new QLabel(tr("  Sear&ch  "), this);
    lblSearch->setBuddy(searchLine);
    ui->topToolBar->addWidget(lblSearch);
    ui->topToolBar->addWidget(searchLine);
}

void MainWindow::populateContextTree()
{
    ui->contextTree->clear();
    QList<QTreeWidgetItem*> children;
    for (auto i : collectContexts()){
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setText(0, QString::fromStdString(i));
        children << child;
    }

    QTreeWidgetItem *root = new QTreeWidgetItem();
    root->setText(0, tr("All"));
    root->addChildren(children);
    ui->contextTree->addTopLevelItem(root);
    ui->contextTree->expandAll();
    ui->languageTable->resizeColumnsToContents();
}

std::vector<std::string> MainWindow::collectContexts()
{
    std::vector<std::string> contexts;
    for (auto i : Language::getKeys()) {
        std::string tmpContext = i.getContext();
        if (std::find(contexts.begin(), contexts.end(), tmpContext) == contexts.end())
            contexts.push_back(tmpContext);
    }
    return contexts;
}

void MainWindow::on_contextTree_itemClicked(QTreeWidgetItem *item, int column)
{
    currentContext = "";
    if (item->childCount() == 0) //not a root item
        currentContext = item->text(column).toStdString();
    updateLanguageTable();
}

void MainWindow::searchString(const QString &s)
{
    sortFilter->setFilterRegExp(QString("^.*(%1).*$").arg(s));
    updateLanguageTable();
}

void MainWindow::updateLanguageTable()
{
    sortFilter->setSourceModel(tableManager.getTableByContext(currentContext));
    ui->languageTable->update();
    ui->languageTable->resizeColumnsToContents();
}

void MainWindow::on_actionExport_Languages_triggered()
{
    exportLanguagesDialog dialog;
    dialog.populateLanguagesList(tableManager.getLanguagesName());
    if (dialog.exec() == QDialog::Accepted)
    {
        CSVwriter writer;
        QString destFilename = QFileDialog::getSaveFileName(this, tr("Import languages"),  workingDirectory,supportedType); //TODO: aggiungerlo
        if (destFilename.isEmpty())
            return;
        writer.setKeys(Language::getKeys());
        for (auto i : tableManager.getLanguages(dialog.languagesToExport())) {
            writer.addLanguage(i);
        }
        writer.save(destFilename);
    }
}
