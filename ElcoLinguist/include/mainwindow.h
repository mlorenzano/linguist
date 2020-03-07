#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "languagestablemanager.h"

#include <QDir>
#include <QMainWindow>

#include <memory>
#include <set>

class LanguagesManager;
class LanguagesManagerModel;

QT_BEGIN_NAMESPACE
class QLineEdit;
class QTreeWidgetItem;
class QLabel;
class QSortFilterProxyModel;
class QTranslator;
QT_END_NAMESPACE

static const QString languagePathHandler
    = QString("%1/%2").arg(QDir::currentPath()).arg("translations");

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

    void changeEvent(QEvent *e) override;

private slots:
    void importFile();

    //    void on_actionExport_triggered();

    //    void on_actionPreferences_triggered();

    //    void on_actionAdd_Language_triggered();

    //    void on_actionRemove_Languages_triggered();

    //    void on_actionFilters_triggered();

    //    void on_actionAbout_triggered();

    //    void on_contextTree_itemClicked(QTreeWidgetItem *item, int column);

    //    void on_actionExport_Languages_triggered();

    void resizeTable();

    void translateApp();

private:
    void populateContextTree();
    std::map<std::string, std::set<std::string>> collectContexts();
    void updateLanguageTable();
    void searchString(const QString &s);

    Ui::MainWindow *ui;
    QLineEdit *searchLine;
    QLabel *lblSearch;
    languagesTableManager tableManager;
    QString supportedType;
    std::string currentContext;
    std::string currentPage;
    std::vector<std::string> filteredLanguages;
    std::unique_ptr<QTranslator> translator;
    QSortFilterProxyModel *sortFilter;

    void loadSettings() noexcept;
    void createActions() noexcept;
    void enableButtons();
};

#endif // MAINWINDOW_H
