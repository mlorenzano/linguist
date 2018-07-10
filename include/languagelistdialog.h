#ifndef EXPORTLANGUAGESDIALOG_H
#define EXPORTLANGUAGESDIALOG_H

#include <QDialog>

namespace Ui {
class exportLanguagesDialog;
}

class languageListDialog : public QDialog
{
    Q_OBJECT

public:
    languageListDialog (const QString &titlem, QWidget *parent = 0);
    explicit languageListDialog(QWidget *parent = 0);
    void populateLanguagesList(const std::vector<std::string> &languages);
    std::vector<std::string> checkedLanguages();
    ~languageListDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::exportLanguagesDialog *ui;
};

#endif // EXPORTLANGUAGESDIALOG_H
