#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QApplication>
#include <QIcon>

#include <ctime>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("About"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowIcon(QIcon(":/el.ico"));

    ui->lblTitle->setText(qApp->applicationName());
    ui->lblVersion->setText(qApp->applicationVersion());

    const auto theTime = time(nullptr);
    const auto y = localtime(&theTime)->tm_year + 1900;
    const auto year = QString::number(y, 10);
    const auto s = QStringLiteral("<p>Copyright &copy; %1 Elco Elettronica.</p>").arg(year);
    ui->lblCopyright->setText(s);

    ui->lblSite->setOpenExternalLinks(true);
    ui->lblSite->setText(
        QStringLiteral("<a href=\"http://%1\">%2</a>")
            .arg(QApplication::organizationDomain(), QApplication::organizationName()));
}

void AboutDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
