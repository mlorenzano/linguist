#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "languagestablemanager.h"
#include "csv.h"
#include <QMainWindow>
#include <QTreeWidget>

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

private:
    void createToolBar();
    void populateContextTree();

    Ui::MainWindow *ui;
    languagesTableManager tableManager;
    QString supportedType;
    std::string currentContext;
};

#endif // MAINWINDOW_H
