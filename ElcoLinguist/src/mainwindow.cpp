#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "aboutdialog.h"
#include "customitemdelegate.h"
#include "filereader.h"
#include "filewriter.h"
#include "language.h"
#include "languagelistdialog.h"
#include "settingsdialog.h"
#include "tablefilter.h"
#include "searchform.h"

#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>
#include <QTreeWidget>
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadSettings();
    createActions();
    translateApp();
    createSearchWidget();
    enableButtons();

    ui->topToolBar->installEventFilter(this);
    ui->menuBar->installEventFilter(this);

    connect(ui->contextTree, &QTreeWidget::itemClicked, this, &MainWindow::contextTreeFilter);
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

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (dynamic_cast<QToolBar*>(watched) != nullptr ||
        dynamic_cast<QMenuBar*>(watched) != nullptr) {
        if (event->type() == QEvent::ContextMenu) {
            return true;
        }
    }
    return false;
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

    setupModel();
    populateContextTree();
    enableButtons();
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

    auto writer = new FileWriter(destFilename, m_languagesModel.languages(), this);
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
    dialog.populateLanguagesList(m_languagesModel.languageNames());

    if (dialog.exec() == QDialog::Accepted) {
        m_languagesModel.removeLanguages(dialog.checkedLanguages());
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

void MainWindow::contextTreeFilter(QTreeWidgetItem *item, int column)
{
    QString context;
    QString page;

    if (item->parent()) {
        if (item->childCount() == 0) {
            if (item->text(column) == QStringLiteral("DynamicStrings") ||
                item->text(column) == QStringLiteral("EventsHandler")) {
                context = "$$" + item->text(column) + "$$";
            } else {
                context = item->parent()->text(column);
                page = item->text(column);
            }
        }
        else {
            context = item->text(column);
        }
    }

    searchContext(context, page);
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

QAction *MainWindow::actionAt(MainWindow::ActionType type)
{
    auto it = std::find_if(m_actions.begin(), m_actions.end(), [&](auto &item) {
        return item.first == type;
    });

    if (it == m_actions.end()) {
        return nullptr;
    }
    return it->second;
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
        act->setIcon(QIcon(":/import_csv.png"));
        connect(act, &QAction::triggered, this, &MainWindow::importFile);
        ui->topToolBar->addAction(act);
        ui->menuFile->addAction(act);
        m_actions << qMakePair<ActionType, QAction *>(ActionType::Import, act);
    }

    {
        auto act = new QAction(this);
        act->setText(tr("Export File..."));
        act->setIcon(QIcon(":/export_csv.png"));
        connect(act, &QAction::triggered, this, &MainWindow::exportFile);
        ui->topToolBar->addAction(act);
        ui->menuFile->addAction(act);
        m_actions << qMakePair<ActionType, QAction *>(ActionType::Export, act);
    }

    ui->topToolBar->addSeparator();

    {
        auto act = new QAction(this);
        act->setText(tr("Add Language"));
        act->setIcon(QIcon(":/add_language.png"));
        connect(act, &QAction::triggered, this, &MainWindow::addLanguage);
        ui->topToolBar->addAction(act);
        ui->menuEdit->addAction(act);
        m_actions << qMakePair<ActionType, QAction *>(ActionType::AddLanguage, act);
    }

    {
        auto act = new QAction(this);
        act->setText(tr("Remove Language"));
        act->setIcon(QIcon(":/delete_language.png"));
        connect(act, &QAction::triggered, this, &MainWindow::removelanguage);
        ui->topToolBar->addAction(act);
        ui->menuEdit->addAction(act);
        m_actions << qMakePair<ActionType, QAction *>(ActionType::RemoveLanguage, act);
    }

    {
        auto act = new QAction(this);
        act->setText(tr("Exit"));
        act->setIcon(QIcon(":/exit.png"));
        connect(act, &QAction::triggered, this, &MainWindow::close);
        ui->menuFile->addAction(act);
        m_actions << qMakePair<ActionType, QAction *>(ActionType::Exit, act);
    }

    ui->topToolBar->addSeparator();

    {
        auto act = new QAction(this);
        act->setText(tr("Settings..."));
        act->setIcon(QIcon(":/settings.png"));
        connect(act, &QAction::triggered, this, &MainWindow::openSettings);
        ui->topToolBar->addAction(act);
        ui->menuTools->addAction(act);
        m_actions << qMakePair<ActionType, QAction *>(ActionType::OpenSettings, act);
    }

    {
        auto act = new QAction(this);
        act->setText(tr("About..."));
        act->setIcon(QIcon(":/el.ico"));
        connect(act, &QAction::triggered, this, &MainWindow::openAbout);
        ui->menuHelp->addAction(act);
        m_actions << qMakePair<ActionType, QAction *>(ActionType::OpenAbout, act);
    }
}

void MainWindow::enableButtons()
{
    const auto fileLoaded = !m_languagesModel.languageNames().isEmpty();
    actionAt(ActionType::Export)->setEnabled(fileLoaded);
    actionAt(ActionType::AddLanguage)->setEnabled(fileLoaded);
    actionAt(ActionType::RemoveLanguage)->setEnabled(fileLoaded);
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

    ui->topToolBar->addWidget(&m_searchform);

//    connect(m_leSearch, &QLineEdit::textEdited, this, &MainWindow::searchString);
//    connect(m_cbCaseSentive, &QCheckBox::stateChanged, this, &MainWindow::setCaseSesitivity);
}

void MainWindow::setupModel()
{
    m_filterSearch = std::make_unique<TableFilter>(Language::getKeys());
    m_filterSearch->setSourceModel(&m_languagesModel);
    ui->languageTable->setModel(m_filterSearch.get());
}

//void MainWindow::setCaseSesitivity()
//{
//    if (m_cbCaseSentive->isChecked())
//        m_filterSearch->setFilterCaseSensitivity(Qt::CaseSensitive);
//    else
//        m_filterSearch->setFilterCaseSensitivity(Qt::CaseInsensitive);
//}

void MainWindow::searchString(const QString &s)
{
    QString regEx;

    if (!s.isEmpty()) {
        regEx = QString("^.*(%1).*$").arg(s);
    }

    m_filterSearch->setFilterRegExp(regEx);
}

void MainWindow::searchContext(const QString &context, const QString &page)
{
    m_filterSearch->setFilter(context, page);
}

void MainWindow::populateContextTree()
{
    ui->contextTree->clear();
    QList<QTreeWidgetItem *> contexts;
    for (auto i : collectContexts()) {
        auto normalContext = true;
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
                const auto childPage = new QTreeWidgetItem;
                childPage->setText(0, QString::fromStdString(j));
                pages << childPage;
            }
            childContext->addChildren(pages);
        }
        contexts << childContext;
    }

    const auto root = new QTreeWidgetItem;
    root->setText(0, tr("All"));
    root->addChildren(contexts);
    ui->contextTree->addTopLevelItem(root);
    ui->contextTree->expandItem(root);
    ui->languageTable->resizeColumnsToContents();
}

std::map<std::string, std::set<std::string>> MainWindow::collectContexts() const noexcept
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
