#include "prosetpage.h"
#include "ui_prosetpage.h"

ProSetPage::ProSetPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    // 注册字段，只有两个字段都有值才能进行下一步
    registerField("proPath", ui->lineEdit_2);
    registerField("proName", ui->lineEdit);
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    connect(ui->lineEdit_2, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    // 设置默认项目路径为当前路径
    QString curPath = QDir::currentPath();
    ui->lineEdit_2->setText(curPath);
    // 设置光标位置
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());
    // 显示清除按钮
    ui->lineEdit_2->setClearButtonEnabled(true);
    ui->lineEdit->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

bool ProSetPage::isComplete() const
{
    if (ui->lineEdit->text() == "" || ui->lineEdit_2->text() == "") {
        return false;
    }

    QDir dir(ui->lineEdit_2->text());
    if (!dir.exists()) {
        ui->tips->setText(tr("项目路径不存在！"));
        return false;
    }

    // 判断路径
    QString absFilePath = dir.absoluteFilePath(ui->lineEdit->text());
    QDir dist_dir(absFilePath);
    if (dist_dir.exists()) {
        ui->tips->setText(tr("项目已经存在，请更改项目路径或项目名称！"));
        return false;
    }

    ui->tips->setText("");
    return QWizardPage::isComplete();
}
