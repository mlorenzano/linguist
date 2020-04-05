#pragma once

#include "LanguagesModel.hpp"
#include "searchform.h"
#include "LanguagesContainer.hpp"

#include <QCheckBox>
#include <QMainWindow>
#include <QTranslator>

#include <memory>
#include <set>

class LanguagesManager;
class LanguagesManagerModel;
class TableFilter;

QT_BEGIN_NAMESPACE
class QLineEdit;
class QTreeWidgetItem;
class QLabel;
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
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void importFile();
    void exportFile();
    void addLanguage();
    void removelanguage();
    void openAbout();
    void openSettings();
    void openSelectLanguages();
    void showStartExport();
    void showFinishExport();
    void contextTreeFilter(QTreeWidgetItem *item, int column);
    void searchString(const QString &s);
    void findAndReplace();
    void setCaseSesitivity(int value);
    void addLanguageFromFile();

private:
    const int kButtonSize{24};
    Ui::MainWindow *ui;
    LanguagesContainer m_containerOfLanguages;
    LanguagesModel m_languagesModel;

    QTranslator m_translator;
    QProgressDialog *m_progressDialog;
    std::unique_ptr<TableFilter> m_filterSearch;
    searchform m_searchform;
    std::vector<std::string> m_filteredLanguages;

    enum class ActionType {
        Import,
        Export,
        AddLanguage,
        RemoveLanguage,
        FindAndReplace,
        OpenAbout,
        OpenSettings,
        Exit,
        SelectLanguages,
        AddLanguageFromFile,

        PlaceholderLast
    };

    QVector<QPair<ActionType, QAction *>> m_actions;
    QAction *actionAt(ActionType type);

    void loadSettings() noexcept;
    void createActions() noexcept;
    void enableButtons();
    void resizeTable();
    void translateApp();
    void createSearchWidget();
    void setupModel();
    void searchContext(const QString &context, const QString &page);
    void setIndexFilteredLang();
    void populateContextTree();
    std::map<std::string, std::set<std::string>> collectContexts() const noexcept;
};
