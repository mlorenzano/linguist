#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "languagestablemanager.h"
#include "csv.h"
#include <QMainWindow>

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
    
private:
    void populateTable();
    csv csvReader;
    Ui::MainWindow *ui;
    void createToolBar();
    languagesTableManager tableManager;
    QString supportedType;
    std::vector<Key> collectKeys();
    std::vector<std::string> collectColumnAt(std::size_t i);
    std::vector<std::string> collectIntestations();
};

#endif // MAINWINDOW_H
