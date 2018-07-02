#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    Ui::MainWindow *ui;
    void createToolBar(); 
};

#endif // MAINWINDOW_H
