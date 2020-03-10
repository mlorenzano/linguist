#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "QSettings"

settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowIcon(QIcon(":/el.ico"));
    ui->languageBox->addItem("Italiano", "it");
    ui->languageBox->addItem("English", "en");
    QSettings set;
    const auto currentLanguage = set.value(currentLanguageHandler, "en").toString();
    auto language_index = ui->languageBox->findData(currentLanguage);
    if (language_index > -1)
        ui->languageBox->setCurrentIndex(language_index);
}

settingsDialog::~settingsDialog()
{
    delete ui;
}

void settingsDialog::on_buttonBox_accepted()
{

    QSettings set;
    const auto language_index = ui->languageBox->currentIndex();
    const auto filename = ui->languageBox->itemData(language_index).toString();
    if (!filename.isEmpty())
        set.setValue(currentLanguageHandler, filename);
}
