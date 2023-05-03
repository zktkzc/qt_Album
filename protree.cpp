#include "protree.h"
#include "ui_protree.h"

ProTree::ProTree(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProTree)
{
    ui->setupUi(this);
    setMinimumWidth(378);
    setMaximumWidth(378);
}

ProTree::~ProTree()
{
    delete ui;
}

void ProTree::addProToTree(const QString name, const QString path)
{
    ui->treeWidget->addProToTree(name, path);
}
