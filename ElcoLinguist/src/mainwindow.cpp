#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutdialog.h"
#include "customitemdelegate.h"
#include "filereader.h"
#include "filewriter.h"
#include "language.h"
#include "languagelistdialog.h"
#include "settingsdialog.h"

#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QTranslator>
#include <QTreeWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , searchLine(new QLineEdit())
    , lblSearch(new QLabel())
    , tableManager()
    , supportedType{"Text CSV (*.csv);;Microsoft Excel 20007-2013 XML (*.xlsx)"}
    , filteredLanguages()
    , translator(new QTranslator)
    , sortFilter(new QSortFilterProxyModel())
{
    ui->setupUi(this);

    loadSettings();
    createActions();
    translateApp();

    //    connect(searchLine, &QLineEdit::textEdited, this, &MainWindow::searchString);
    //    currentContext = "";
    //    sortFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    //    sortFilter->setSourceModel(
    //        tableManager.getTable(currentContext, currentPage, filteredLanguages));
    ui->languageTable->setModel(sortFilter);
    ui->languageTable->setItemDelegate(new CustomItemDelegate);
    connect(&tableManager, &languagesTableManager::dataChanged, this, &MainWindow::resizeTable);

    //    QWidget *empty = new QWidget();
    //    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    //    ui->topToolBar->addWidget(empty);

    //    lblSearch->setBuddy(searchLine);
    //    ui->topToolBar->addWidget(lblSearch);
    //    ui->topToolBar->addWidget(searchLine);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

void MainWindow::importFile()
{
    QSettings set;
    const auto destFilename
        = QFileDialog::getOpenFileName(this,
                                       tr("Import languages"),
                                       set.value("workingDirectory", QString()).toString(),
                                       supportedType)
              .toStdString();

    if (destFilename.empty()) {
        return;
    }

    tableManager.clear();

    QFileInfo info(QString::fromStdString(destFilename));
    setWindowTitle(info.fileName() + " - " + "ElcoLinguist");
    set.setValue("workingDirectory", info.path());
    FileReader reader(destFilename);
    auto a = reader.collectKeys();
    Language::setKeys(a);
    std::vector<std::string> intestations = reader.collectIntestations();
    if (!intestations.empty()) {
        tableManager.setDefault(Language(intestations[1], reader.collectColumnAt(1)));
    }

    for (size_t i = 1; i < intestations.size(); ++i) {
        tableManager.insertLanguage(intestations[i],
                                    Language(intestations[i], reader.collectColumnAt(i)));
    }

    currentContext = "";
    populateContextTree();
    filteredLanguages = tableManager.getLanguagesName();
    enableButtons();
    updateLanguageTable();
}

void MainWindow::exportFile()
{
    QString destFilename = QFileDialog::getSaveFileName(this,
                                                        tr("Export languages"),
                                                        QString(),
                                                        supportedType);
    if (destFilename.isEmpty())
        return;

    FileWriter().save(destFilename.toStdString(), tableManager.getLanguages());
}

void MainWindow::openAbout()
{
    AboutDialog().exec();
}

void MainWindow::openSettings()
{
    settingsDialog().exec();
    translateApp();
}

//void MainWindow::on_actionAdd_Language_triggered()
//{
//    std::string name = QInputDialog::getText(this, tr("New language"), tr("Insert language name:"))
//                           .toStdString();
//    if (name.empty())
//        return;
//    if (!tableManager.insertLanguage(name, Language(name)))
//        QMessageBox::information(this, tr("Error"), tr("This language already exists."));
//    else {
//        filteredLanguages.push_back(name);
//        ui->actionRemove_Languages->setEnabled(true);
//        ui->actionFilters->setEnabled(true);
//    }
//    updateLanguageTable();
//}

//void MainWindow::on_actionRemove_Languages_triggered()
//{
//    languageListDialog dialog(tr("Remove Languages"));
//    dialog.populateLanguagesList(tableManager.getLanguagesName());
//    if (dialog.exec() == QDialog::Accepted) {
//        std::vector<std::string> languagesToRemove = dialog.checkedLanguages();
//        tableManager.removeLanguages(languagesToRemove);
//        for (auto i : languagesToRemove) {
//            auto position = std::find(filteredLanguages.begin(), filteredLanguages.end(), i);
//            if (position != filteredLanguages.end())
//                filteredLanguages.erase(position);
//        }
//        if (tableManager.getLanguagesName().size() == 0) {
//            ui->actionRemove_Languages->setEnabled(false);
//            ui->actionFilters->setEnabled(false);
//        }
//    }
//    updateLanguageTable();
//}

//void MainWindow::on_actionFilters_triggered()
//{
//    languageListDialog dialog(tr("Set Filters"));
//    dialog.populateLanguagesList(tableManager.getLanguagesName());
//    dialog.setSelectedLanguages(filteredLanguages);
//    if (dialog.exec() == QDialog::Accepted) {
//        filteredLanguages = dialog.checkedLanguages();
//    }
//    updateLanguageTable();
//}

//void MainWindow::on_actionExport_Languages_triggered()
//{
//    languageListDialog dialog(tr("Export Languages"));
//    dialog.populateLanguagesList(tableManager.getLanguagesName());
//    if (dialog.exec() == QDialog::Accepted) {
//        QString destFilename = QFileDialog::getSaveFileName(this,
//                                                            tr("Import languages"),
//                                                            QString(),
//                                                            supportedType);
//        if (destFilename.isEmpty())
//            return;
//        FileWriter writer(destFilename.toStdString());
//        writer.setKeys(Language::getKeys());
//        writer.addLanguages(tableManager.getLanguages(dialog.checkedLanguages()));
//        writer.save();
//    }
//}

void MainWindow::populateContextTree()
{
    ui->contextTree->clear();
    QList<QTreeWidgetItem *> contexts;
    bool normalContext;
    for (auto i : collectContexts()) {
        normalContext = true;
        QTreeWidgetItem *childContext = new QTreeWidgetItem();
        if (i.first == "$$DynamicStrings$$") {
            childContext->setText(0, "DynamicStrings");
            normalContext = false;
        } else if (i.first == "$$EventsHandler$$") {
            childContext->setText(0, "EventsHandler");
            normalContext = false;
        } else
            childContext->setText(0, QString::fromStdString(i.first));
        if (normalContext) {
            QList<QTreeWidgetItem *> pages;
            for (auto j : i.second) {
                QTreeWidgetItem *childPage = new QTreeWidgetItem();
                childPage->setText(0, QString::fromStdString(j));
                pages << childPage;
            }
            childContext->addChildren(pages);
        }
        contexts << childContext;
    }

    QTreeWidgetItem *root = new QTreeWidgetItem();
    root->setText(0, tr("All"));
    root->addChildren(contexts);
    ui->contextTree->addTopLevelItem(root);
    ui->contextTree->expandItem(root);
    ui->languageTable->resizeColumnsToContents();
}
std::map<std::string, std::set<std::string>> MainWindow::collectContexts()
{
    std::map<std::string, std::set<std::string>> contexts;
    for (auto i : Language::getKeys()) {
        std::string tmpContext = i.getContext();
        if (contexts.find(tmpContext) == contexts.end())
            contexts.insert(std::make_pair(tmpContext, std::set<std::string>{i.getPageOfContext()}));
        else {
            if (contexts.at(tmpContext).find(i.getPageOfContext()) == contexts.at(tmpContext).end())
                contexts.at(tmpContext).insert(i.getPageOfContext());
        }
    }
    return contexts;
}

//void MainWindow::on_contextTree_itemClicked(QTreeWidgetItem *item, int column)
//{
//    if (item->childCount() == 0) {
//        if (item->text(column) == "DynamicStrings" || item->text(column) == "EventsHandler") {
//            currentContext = "$$" + item->text(column).toStdString() + "$$";
//            currentPage = "";
//        } else {
//            currentPage = item->text(column).toStdString();
//            currentContext = item->parent()->text(column).toStdString();
//        }
//    } else {
//        currentPage = "";
//        if (item->parent())
//            currentContext = item->text(column).toStdString();
//        else
//            currentContext = "";
//    }
//    updateLanguageTable();
//}

void MainWindow::searchString(const QString &s)
{
    sortFilter->setFilterRegExp(QString("^.*(%1).*$").arg(s));
    updateLanguageTable();
}

void MainWindow::updateLanguageTable()
{
    sortFilter->setSourceModel(
        tableManager.getTable(currentContext, currentPage, filteredLanguages));
    ui->languageTable->update();
    resizeTable();
}

void MainWindow::loadSettings() noexcept
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::createActions() noexcept
{
    {
        auto act = new QAction(this);
        act->setText(tr("Import File..."));
        act->setIcon(QIcon(":/gnome_import.png"));
        connect(act, &QAction::triggered, this, &MainWindow::importFile);
        ui->topToolBar->addAction(act);
        ui->menuFile->addAction(act);
    }

    {
        auto act = new QAction(this);
        act->setText(tr("Export File..."));
        act->setIcon(QIcon(":/gnome_export.png"));
        connect(act, &QAction::triggered, this, &MainWindow::exportFile);
        ui->topToolBar->addAction(act);
        ui->menuFile->addAction(act);
    }

    {
        auto act = new QAction(this);
        act->setText(tr("About..."));
        connect(act, &QAction::triggered, this, &MainWindow::openAbout);
        ui->menuHelp->addAction(act);
    }

    {
        auto act = new QAction(this);
        act->setText(tr("Settings..."));
        connect(act, &QAction::triggered, this, &MainWindow::openSettings);
        ui->menuTools->addAction(act);
    }
}

void MainWindow::enableButtons()
{
    // FIXME: implement this feature correctly
    //    ui->actionRemove_Languages->setEnabled(tableManager.getLanguagesName().size() != 0);
    //    ui->actionFilters->setEnabled(tableManager.getLanguagesName().size() != 0);
    //    ui->actionAdd_Language->setEnabled(true);
    //    ui->actionExport->setEnabled(true);
    //    ui->actionExport_Languages->setEnabled(true);
}

void MainWindow::resizeTable()
{
    ui->languageTable->resizeColumnsToContents();
    ui->languageTable->resizeRowsToContents();
}

void MainWindow::translateApp()
{
    qApp->removeTranslator(translator.get());
    QSettings set;
    auto currentLanguage = set.value(currentLanguageHandler, "en").toString();
    auto qmToLoad = QString(":/elcolinguist_%1.qm").arg(currentLanguage);
    if (translator->load(qmToLoad)) {
        qApp->installTranslator(translator.get());
    }
}
