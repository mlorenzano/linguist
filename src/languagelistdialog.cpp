#include "include\languagelistdialog.h"
#include "ui_exportlanguagesdialog.h"

languageListDialog::languageListDialog (const QString &title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exportLanguagesDialog)
{
    ui->setupUi(this);
    QDialog::setWindowTitle(title);
}

languageListDialog::languageListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exportLanguagesDialog)
{
    ui->setupUi(this);
}

languageListDialog::~languageListDialog()
{
    delete ui;
}

void languageListDialog::populateLanguagesList(const std::vector<std::string> &languages)
{
    auto *def = new QListWidgetItem();
    def->setText("Default");
    def->setCheckState(Qt::Checked);
    def->setFlags(def->flags() & ~Qt::ItemIsEnabled);
    ui->languagesListWidget->addItem(def);

    for (auto lang : languages) {
        auto *item = new QListWidgetItem();
        item->setText(QString::fromStdString(lang));
        item->setCheckState(Qt::Unchecked);
        ui->languagesListWidget->addItem(item);
    }
}

std::vector<std::string> languageListDialog::checkedLanguages()
{
    std::vector<std::string> checkedLanguages;
    for (int i = 1; i < ui->languagesListWidget->count(); i++) {
        auto item = ui->languagesListWidget->item(i);
        if (item->checkState() == Qt::Checked)
            checkedLanguages.push_back(item->text().toStdString());
        item->setCheckState(Qt::Unchecked);
    }
    return checkedLanguages;
}

void languageListDialog::on_buttonBox_accepted()
{
    accept();
}

void languageListDialog::on_buttonBox_rejected()
{
    reject();
}
