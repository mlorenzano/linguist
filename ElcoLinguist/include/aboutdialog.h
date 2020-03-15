#pragma once

#include <QDialog>

namespace Ui {
class AboutDialog;
} // namespace Ui

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AboutDialog *ui;
};
