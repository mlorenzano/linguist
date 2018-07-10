#include "include\exportlanguagesdialog.h"
#include "ui_exportlanguagesdialog.h"

exportLanguagesDialog::exportLanguagesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exportLanguagesDialog)
{
    ui->setupUi(this);
}

exportLanguagesDialog::~exportLanguagesDialog()
{
    delete ui;
}

void exportLanguagesDialog::populateLanguagesList(const std::vector<std::string> &languages)
{
    for (auto lang : languages) {
        auto *item = new QListWidgetItem();
        item->setText(lang);
        item->setCheckState(Qt::Unchecked);
    }
}
std::vector<std::string> exportLanguagesDialog::languagesToExport()
{
    std::vector<std::string> checkedLanguages;
    for (int i = 0; i < ui->languagesListWidget->size(); i++) {
        auto item = ui->languagesListWidget->item(i);
        if (item->isSelected())
            checkedLanguages.push_back(item->text());
        item->setCheckState(Qt::Unchecked);
    }
}

void exportLanguagesDialog::on_buttonBox_accepted()
{
    accept();
}

void exportLanguagesDialog::on_buttonBox_rejected()
{
    reject();
}
