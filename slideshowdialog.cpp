#include "slideshowdialog.h"
#include "ui_slideshowdialog.h"
#include "prelistwidget.h"

slideShowDialog::slideShowDialog(QWidget *parent, QTreeWidgetItem *first_item, QTreeWidgetItem *last_item) :
    QDialog(parent), m_first_item(first_item), m_last_item(last_item),
    ui(new Ui::slideShowDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->slidePreBtn->setIcons(":/icon/previous.png", ":/icon/previous_hover.png", ":/icon/previous_press.png");
    ui->slideNextBtn->setIcons(":/icon/next.png", ":/icon/next_hover.png", ":/icon/next_press.png");
    ui->closeBtn->setIcons(":/icon/closeshow.png", ":/icon/closeshow_hover.png", ":/icon/closeshow_press.png");
    ui->playBtn->setIcons(":/icon/play.png", ":/icon/play_hover.png", ":/icno/play_press.png", ":/icon/pause.png", ":/icon/pause_hover",
                          ":/icon/pause_press.png");

    auto *preListWidget = dynamic_cast<PreListWidget*>(ui->preListWidget);
    connect(ui->picAnimation, &PicAnimationWidget::sigUpdatePreList, preListWidget, &PreListWidget::slotUpdatePreList);
    connect(ui->picAnimation, &PicAnimationWidget::sigUpdateSelectedItem, preListWidget, &PreListWidget::slotUpdateSelectedItem);
    ui->picAnimation->setPixmap(m_first_item);
    ui->picAnimation->start();
}

slideShowDialog::~slideShowDialog()
{
    delete ui;
}
