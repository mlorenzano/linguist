#pragma once

#include "LanguagesModel.hpp"

#include <QMainWindow>
#include <QTranslator>
#include <QCheckBox>
#include "searchform.h"

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
    void showStartExport();
    void showFinishExport();
    void contextTreeFilter(QTreeWidgetItem *item, int column);
    void searchString(const QString &s);
    void setCaseSesitivity(int value);

private:
    const int kButtonSize{24};
    Ui::MainWindow *ui;
    LanguagesModel m_languagesModel;

    QTranslator m_translator;
    QProgressDialog *m_progressDialog;
    std::unique_ptr<TableFilter> m_filterSearch;
    searchform m_searchform;

    enum class ActionType {
        Import,
        Export,
        AddLanguage,
        RemoveLanguage,
        OpenAbout,
        OpenSettings,
        Exit,

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
    void populateContextTree();
    std::map<std::string, std::set<std::string>> collectContexts() const noexcept;
};
