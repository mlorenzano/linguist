#include "searchform.h"
#include "ui_searchform.h"

searchform::searchform(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::searchform)
{
    ui->setupUi(this);

    connect(ui->lineEdit, &QLineEdit::textEdited, this, &searchform::textChanged);
    connect(ui->checkBox, &QCheckBox::stateChanged, this, &searchform::sensitivityChanged);
}

searchform::~searchform()
{
    delete ui;
}
