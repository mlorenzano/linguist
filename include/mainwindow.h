#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "languagestablemanager.h"
#include "csv.h"

#include <QMainWindow>
#include <QTreeWidget>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include <QTranslator>
#include <QDir>
#include <QLabel>

#include <set>

class LanguagesManager;
class LanguagesManagerModel;

static const QString languagePathHandler =
        QString("%1/%2").arg(QDir::currentPath()).arg("translations");

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    
    void changeEvent(QEvent *e);
private slots:
    void on_actionExport_triggered();
    
    void on_actionImport_triggered();
    
    void on_actionPreferences_triggered();
    
    void on_actionAdd_Language_triggered();
    
    void on_actionRemove_Languages_triggered();
    
    void on_actionFilters_triggered();
    
    void on_actionAbout_triggered();
    
    void on_contextTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_actionExport_Languages_triggered();

    void resizeTable();

    void translateApp();

private:
    void createToolBar();
    void populateContextTree();
    std::map<std::string, std::set<std::string>> collectContexts();
    void updateLanguageTable();
    void searchString(const QString &s);

    Ui::MainWindow *ui;
    QLineEdit *searchLine;
    QLabel *lblSearch;
    languagesTableManager tableManager;
    QString supportedType;
    QString workingDirectory;
    std::string currentContext;
    std::string currentPage;
    std::vector<std::string> filteredLanguages;
    std::unique_ptr<QTranslator> translator;
    QSortFilterProxyModel *sortFilter;
};

#endif // MAINWINDOW_H
