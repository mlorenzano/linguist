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
#include <QProgressDialog>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QTreeWidget>
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_leSearch(new QLineEdit(this))
    , m_lblSearch(new QLabel(this))
    //    , filteredLanguages()
    , m_filterSearch(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);

    loadSettings();
    createActions();
    translateApp();
    createSearchWidget();
    setupModel();

    //    currentContext = "";

    //    ui->languageTable->setItemDelegate(new CustomItemDelegate);
    //    connect(&tableManager, &languagesTableManager::dataChanged, this, &MainWindow::resizeTable);

    //    QWidget *empty = new QWidget();
    //    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    //    ui->topToolBar->addWidget(empty);
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
    if (e->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
}

void MainWindow::importFile()
{
    QSettings set;
    const auto destFilename
        = QFileDialog::getOpenFileName(this,
                                       tr("Import languages"),
                                       set.value("workingDirectory", QString()).toString(),
                                       "Text CSV (*.csv)")
              .toStdString();

    if (destFilename.empty()) {
        return;
    }

    m_languagesModel.clear();

    QFileInfo info(QString::fromStdString(destFilename));
    setWindowTitle(info.fileName() + " - " + "ElcoLinguist");
    set.setValue("workingDirectory", info.path());

    FileReader reader(destFilename);
    auto a = reader.collectKeys();
    Language::setKeys(a);
    const auto intestations = reader.collectIntestations();
    if (!intestations.empty()) {
        m_languagesModel.setDefault(Language(intestations[1], reader.collectColumnAt(1)));
    }

    for (size_t i = 1; i < intestations.size(); ++i) {
        m_languagesModel.insertLanguage(intestations[i],
                                        Language(intestations[i], reader.collectColumnAt(i)));
    }

    //    currentContext = "";
    //    populateContextTree();
    //    filteredLanguages = tableManager.getLanguagesName();
    enableButtons();
    //    updateLanguageTable();
    resizeTable();
}

void MainWindow::exportFile()
{
    QString destFilename = QFileDialog::getSaveFileName(this,
                                                        tr("Export languages"),
                                                        QString(),
                                                        "Text CSV (*.csv)");
    if (destFilename.isEmpty())
        return;

    auto writer = new FileWriter(destFilename.toStdString(), tableManager.getLanguages(), this);
    writer->setAutoDelete(true);
    QThreadPool::globalInstance()->start(writer, QThread::HighPriority);
}

void MainWindow::addLanguage()
{
    const auto name = QInputDialog::getText(this, tr("New language"), tr("Insert language name:"))
                          .toStdString();
    if (name.empty()) {
        return;
    }

    if (m_languagesModel.exists(name)) {
        QMessageBox::information(this, tr("Error"), tr("This language already exists."));
    }

    m_languagesModel.insertLanguage(name, Language(name));
}

void MainWindow::removelanguage()
{
    languageListDialog dialog(tr("Remove Languages"));
    //    dialog.populateLanguagesList(tableManager.getLanguagesName());

    if (dialog.exec() == QDialog::Accepted) {
        std::vector<std::string> languagesToRemove = dialog.checkedLanguages();
        //        tableManager.removeLanguages(languagesToRemove);
        for (auto i : languagesToRemove) {
            //            auto position = std::find(filteredLanguages.begin(), filteredLanguages.end(), i);
            //            if (position != filteredLanguages.end())
            //                filteredLanguages.erase(position);
        }
        updateLanguageTable();
    }
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

void MainWindow::showStartExport()
{
    m_progressDialog = new QProgressDialog(tr("Exporting file..."),
                                           "",
                                           0,
                                           0,
                                           this,
                                           Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    m_progressDialog->setCancelButton(nullptr);
    m_progressDialog->exec();
}

void MainWindow::showFinishExport()
{
    QThreadPool::globalInstance()->releaseThread();

    m_progressDialog->close();
    if (m_progressDialog)
        delete m_progressDialog;

    QMessageBox::information(this, tr("Finished!"), tr("Export file succesfully."));
}

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
    //    ui->contextTree->clear();
    //    QList<QTreeWidgetItem *> contexts;
    //    bool normalContext;
    //    for (auto i : collectContexts()) {
    //        normalContext = true;
    //        QTreeWidgetItem *childContext = new QTreeWidgetItem();
    //        if (i.first == "$$DynamicStrings$$") {
    //            childContext->setText(0, "DynamicStrings");
    //            normalContext = false;
    //        } else if (i.first == "$$EventsHandler$$") {
    //            childContext->setText(0, "EventsHandler");
    //            normalContext = false;
    //        } else
    //            childContext->setText(0, QString::fromStdString(i.first));
    //        if (normalContext) {
    //            QList<QTreeWidgetItem *> pages;
    //            for (auto j : i.second) {
    //                QTreeWidgetItem *childPage = new QTreeWidgetItem();
    //                childPage->setText(0, QString::fromStdString(j));
    //                pages << childPage;
    //            }
    //            childContext->addChildren(pages);
    //        }
    //        contexts << childContext;
    //    }

    //    QTreeWidgetItem *root = new QTreeWidgetItem();
    //    root->setText(0, tr("All"));
    //    root->addChildren(contexts);
    //    ui->contextTree->addTopLevelItem(root);
    //    ui->contextTree->expandItem(root);
    //    ui->languageTable->resizeColumnsToContents();
}

//std::map<std::string, std::set<std::string>> MainWindow::collectContexts()
//{
//    std::map<std::string, std::set<std::string>> contexts;
//    for (auto i : Language::getKeys()) {
//        std::string tmpContext = i.getContext();
//        if (contexts.find(tmpContext) == contexts.end())
//            contexts.insert(std::make_pair(tmpContext, std::set<std::string>{i.getPageOfContext()}));
//        else {
//            if (contexts.at(tmpContext).find(i.getPageOfContext()) == contexts.at(tmpContext).end())
//                contexts.at(tmpContext).insert(i.getPageOfContext());
//        }
//    }
//    return contexts;
//}

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
    m_filterSearch->setFilterRegExp(QString("^.*(%1).*$").arg(s));
}

void MainWindow::updateLanguageTable()
{
    //    ui->languageTable->update();
    //    resizeTable();
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

    ui->topToolBar->addSeparator();

    {
        auto act = new QAction(this);
        act->setText(tr("Add Language"));
        act->setIcon(QIcon(":/gnome_add.png"));
        connect(act, &QAction::triggered, this, &MainWindow::addLanguage);
        ui->topToolBar->addAction(act);
        ui->menuEdit->addAction(act);
    }

    {
        auto act = new QAction(this);
        act->setText(tr("Remove Language"));
        act->setIcon(QIcon(":/gnome_remove.png"));
        connect(act, &QAction::triggered, this, &MainWindow::removelanguage);
        ui->topToolBar->addAction(act);
        ui->menuEdit->addAction(act);
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
    qApp->removeTranslator(&m_translator);
    QSettings set;
    auto currentLanguage = set.value(currentLanguageHandler, "en").toString();
    auto qmToLoad = QString(":/elcolinguist_%1.qm").arg(currentLanguage);
    if (m_translator.load(qmToLoad)) {
        qApp->installTranslator(&m_translator);
    }
}

void MainWindow::createSearchWidget()
{
    connect(m_leSearch, &QLineEdit::textEdited, this, &MainWindow::searchString);
    m_lblSearch->setBuddy(m_leSearch);
    m_lblSearch->setText(tr("Search:"));
    ui->topToolBar->addWidget(m_lblSearch);
    ui->topToolBar->addWidget(m_leSearch);
}

void MainWindow::setupModel()
{
    m_filterSearch->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_filterSearch->setSourceModel(&m_languagesModel);
    ui->languageTable->setModel(m_filterSearch);
}
