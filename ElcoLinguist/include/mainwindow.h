#pragma once

#include "LanguagesModel.hpp"

#include <QMainWindow>
#include <QTranslator>

#include <set>

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
    Ui::MainWindow *ui;
    LanguagesModel m_languagesModel;

    QLineEdit *m_leSearch;
    QLabel *m_lblSearch;
    //    std::string currentContext;
    //    std::string currentPage;
    //    std::vector<std::string> filteredLanguages;
    QTranslator m_translator;
    QProgressDialog *m_progressDialog;
    QSortFilterProxyModel *m_filterSearch;

    enum class ACTION_TYPE {
        IMPORT,
        EXPORT,
        ADD_LANG,
        REMOVE_LANG,
        OPEN_ABOUT,
        OPEN_SETTINGS,

        PLACEHOLDER_LAST
    };
    QVector<QPair<ACTION_TYPE, QAction *>> m_actions;
    QAction *actionAt(ACTION_TYPE type);

    void loadSettings() noexcept;
    void createActions() noexcept;
    void enableButtons();
    void resizeTable();
    void translateApp();
    void createSearchWidget();
    void setupModel();
    void searchString(const QString &s);
    void populateContextTree();
    std::map<std::string, std::set<std::string>> collectContexts() const noexcept;
};
