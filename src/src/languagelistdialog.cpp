#include "languagelistdialog.h"
#include "ui_exportlanguagesdialog.h"

languageListDialog::languageListDialog(const QString &title, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::exportLanguagesDialog)
{
    ui->setupUi(this);
    QDialog::setWindowTitle(title);
    setWindowIcon(QIcon(":/el.ico"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

languageListDialog::languageListDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::exportLanguagesDialog)
{
    ui->setupUi(this);
}

languageListDialog::~languageListDialog()
{
    delete ui;
}

void languageListDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

void languageListDialog::populateLanguagesList(const QVector<QString> &languages)
{
    for (const auto &lang : languages) {
        auto item = new QListWidgetItem;
        item->setText(lang);
        item->setCheckState(Qt::Unchecked);
        ui->languagesListWidget->addItem(item);
    }
}

void languageListDialog::setSelectedLanguages(const std::vector<std::string> &filteredLanguages)
{
    for (auto lang : filteredLanguages) {
        for (int i = 0; i < ui->languagesListWidget->count(); i++) {
            if (ui->languagesListWidget->item(i)->text().toStdString() == lang) {
                ui->languagesListWidget->item(i)->setCheckState(Qt::Checked);
                break;
            }
        }
    }
}
std::vector<std::string> languageListDialog::checkedLanguages()
{
    std::vector<std::string> checkedLanguages;
    for (int i = 0; i < ui->languagesListWidget->count(); i++) {
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
