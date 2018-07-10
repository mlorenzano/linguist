#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "languagestablemanager.h"
#include "csv.h"

#include <QMainWindow>
#include <QTreeWidget>
#include <QSortFilterProxyModel>
#include <QLineEdit>

class LanguagesManager;
class LanguagesManagerModel;

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionExport_triggered();
    
    void on_actionImport_triggered();
    
    void on_actionPreferences_triggered();
    
    void on_actionAdd_Language_triggered();
    
    void on_actionRemove_Language_triggered();
    
    void on_actionFilters_triggered();
    
    void on_actionAbout_triggered();
    
    void on_contextTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_actionExport_Languages_triggered();

private:
    void createToolBar();
    void populateContextTree();
    std::vector<std::string> collectContexts();
    void updateLanguageTable();

    void searchString(const QString &s);

    Ui::MainWindow *ui;
    QLineEdit *searchLine;
    languagesTableManager tableManager;
    QString supportedType;
    QString workingDirectory;
    std::string currentContext;
    QSortFilterProxyModel *sortFilter;
};

#endif // MAINWINDOW_H
