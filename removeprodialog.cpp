#include "removeprodialog.h"
#include "ui_removeprodialog.h"

RemoveProDialog::RemoveProDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoveProDialog)
{
    ui->setupUi(this);
}

RemoveProDialog::~RemoveProDialog()
{
    delete ui;
}

bool RemoveProDialog::isRemoved()
{
    bool b_checked = ui->checkBox->isChecked();
    return b_checked;
}
