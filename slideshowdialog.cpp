#include "slideshowdialog.h"
#include "ui_slideshowdialog.h"
#include "prelistwidget.h"
#include "picanimationwidget.h"
#include "protreewidget.h"

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

    connect(ui->closeBtn, &QPushButton::clicked, this, &slideShowDialog::close);
    connect(ui->slidePreBtn, &QPushButton::clicked, this, &slideShowDialog::slotSlidePre);
    connect(ui->slideNextBtn, &QPushButton::clicked, this, &slideShowDialog::slotSlideNext);
    connect(ui->playBtn, &QPushButton::clicked, ui->picAnimation, &PicAnimationWidget::slotStartOrStop);
    connect(ui->picAnimation, &PicAnimationWidget::sigStart, ui->playBtn, &PicStateBtn::slotStart);
    connect(ui->picAnimation, &PicAnimationWidget::sigStop, ui->playBtn, &PicStateBtn::slotStop);

    auto *protree_widget = dynamic_cast<ProTreeWidget*>(parent);
    connect(ui->picAnimation, &PicAnimationWidget::sigStartMusic, protree_widget, &ProTreeWidget::slotStartMusic);
    connect(ui->picAnimation, &PicAnimationWidget::sigStopMusic, protree_widget, &ProTreeWidget::slotStopMusic);

    auto *preListWidget = dynamic_cast<PreListWidget*>(ui->preListWidget);
    connect(ui->picAnimation, &PicAnimationWidget::sigUpdatePreList, preListWidget, &PreListWidget::slotUpdatePreList);
    connect(ui->picAnimation, &PicAnimationWidget::sigUpdateSelectedItem, preListWidget, &PreListWidget::slotUpdateSelectedItem);
    connect(preListWidget, &PreListWidget::sigUpdateSelectedShow, ui->picAnimation, &PicAnimationWidget::slotUpdateSelectedShow);
    ui->picAnimation->setPixmap(m_first_item);
    ui->picAnimation->start();
}

slideShowDialog::~slideShowDialog()
{
    delete ui;
}

void slideShowDialog::slotSlidePre()
{
    ui->picAnimation->slidePre();
}

void slideShowDialog::slotSlideNext()
{
    ui->picAnimation->slideNext();
}
