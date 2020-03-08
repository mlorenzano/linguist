#pragma once

#include "LanguagesModel.hpp"

#include <QMainWindow>
#include <QTranslator>

class LanguagesManager;
class LanguagesManagerModel;

QT_BEGIN_NAMESPACE
class QLineEdit;
class QTreeWidgetItem;
class QLabel;
class QSortFilterProxyModel;
class QProgressDialog;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
} // namespace Ui

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
    void exportFile();
    void addLanguage();
    void removelanguage();
    void openAbout();
    void openSettings();
    void showStartExport();
    void showFinishExport();

private:
    void populateContextTree();
    //    std::map<std::string, std::set<std::string>> collectContexts();
    void updateLanguageTable();
    void searchString(const QString &s);

    Ui::MainWindow *ui;
    LanguagesModel m_languagesModel;

    QLineEdit *searchLine;
    QLabel *lblSearch;
    //    std::string currentContext;
    //    std::string currentPage;
    //    std::vector<std::string> filteredLanguages;
    QTranslator m_translator;
    QSortFilterProxyModel *sortFilter;
    QProgressDialog *m_progressDialog;

    void loadSettings() noexcept;
    void createActions() noexcept;
    void enableButtons();
    void resizeTable();
    void translateApp();
};
