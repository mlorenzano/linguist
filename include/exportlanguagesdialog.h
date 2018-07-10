#ifndef EXPORTLANGUAGESDIALOG_H
#define EXPORTLANGUAGESDIALOG_H

#include <QDialog>

namespace Ui {
class exportLanguagesDialog;
}

class exportLanguagesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit exportLanguagesDialog(QWidget *parent = 0);
    void populateLanguagesList(const std::vector<std::string> &languages);
    std::vector<std::string> languagesToExport();
    ~exportLanguagesDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::exportLanguagesDialog *ui;
};

#endif // EXPORTLANGUAGESDIALOG_H
