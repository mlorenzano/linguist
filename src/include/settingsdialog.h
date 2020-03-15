#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

static const QString currentLanguageHandler =
        QLatin1String("Settings/Language/CurrentLanguage");

namespace Ui {
class settingsDialog;
}

class settingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit settingsDialog(QWidget *parent = 0);
    ~settingsDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::settingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
