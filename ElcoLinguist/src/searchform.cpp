#include "searchform.h"
#include "ui_searchform.h"

searchform::searchform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchform)
{
    ui->setupUi(this);
}

searchform::~searchform()
{
    delete ui;
}

void searchform::searchstring()
{

}

void searchform::setcasesensitive()
{

}
