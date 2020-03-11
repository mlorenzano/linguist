#include "aboutdialog.h"
#include "ui_aboutdialog.h"

//#include <updater.h>

#include <QApplication>
#include <QIcon>

#include <time.h>

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

    time_t theTime = time(nullptr);
    const int y = localtime(&theTime)->tm_year + 1900;
    const QString year{QString::number(y, 10)};
    const QString s = QString("<p>Copyright &copy; %1 Elco Elettronica.</p>").arg(year);
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
