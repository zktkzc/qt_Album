#include "picshow.h"
#include "ui_picshow.h"

PicShow::PicShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PicShow)
{
    ui->setupUi(this);
    ui->preBtn->setIcons(":/icon/previous.png", ":/icon/previous_hover.png", ":/icon/previous_press.png");
    ui->nextBtn->setIcons(":/icon/next.png", ":/icon/next_hover.png", ":/icon/next_press.png");

    QGraphicsOpacityEffect *opacity_pre = new QGraphicsOpacityEffect(this);
    opacity_pre->setOpacity(0); // 完全透明
    ui->preBtn->setGraphicsEffect(opacity_pre);

    QGraphicsOpacityEffect *opacity_next = new QGraphicsOpacityEffect(this);
    opacity_next->setOpacity(0); // 完全透明
    ui->nextBtn->setGraphicsEffect(opacity_next);

    m_animation_show_pre = new QPropertyAnimation(opacity_pre, "opacity", this);
    m_animation_show_pre->setEasingCurve(QEasingCurve::Linear);
    m_animation_show_pre->setDuration(500);
    m_animation_show_next = new QPropertyAnimation(opacity_next, "opacity", this);
    m_animation_show_next->setEasingCurve(QEasingCurve::Linear);
    m_animation_show_next->setDuration(500);

    connect(ui->nextBtn, &QPushButton::clicked, this, &PicShow::sigNextClicked);
    connect(ui->preBtn, &QPushButton::clicked, this, &PicShow::sigPreClicked);
}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::reloadPic()
{
    if (m_selected_path != "") {
        const auto &width = ui->gridLayout->geometry().width();
        const auto &height = ui->gridLayout->geometry().height();
        m_pix_map.load(m_selected_path);
        m_pix_map = m_pix_map.scaled(width, height, Qt::KeepAspectRatio);
        ui->label->setPixmap(m_pix_map);
    }
}

bool PicShow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        showPreNextBtns(true);
        break;
    case QEvent::Leave:
        showPreNextBtns(false);
        break;
    default:
        break;
    }

    return QDialog::event(event);
}

void PicShow::showPreNextBtns(bool b_show)
{
    if (!b_show && m_b_btnVisible) {
        m_animation_show_pre->stop();
        m_animation_show_pre->setStartValue(1);
        m_animation_show_pre->setEndValue(0);
        m_animation_show_pre->start();

        m_animation_show_next->stop();
        m_animation_show_next->setStartValue(1);
        m_animation_show_next->setEndValue(0);
        m_animation_show_next->start();

        m_b_btnVisible = false;
        return;
    }

    if (b_show && !m_b_btnVisible) {
        m_animation_show_pre->stop();
        m_animation_show_pre->setStartValue(0);
        m_animation_show_pre->setEndValue(1);
        m_animation_show_pre->start();

        m_animation_show_next->stop();
        m_animation_show_next->setStartValue(0);
        m_animation_show_next->setEndValue(1);
        m_animation_show_next->start();

        m_b_btnVisible = true;
        return;
    }
}

void PicShow::slotSelectedItem(const QString &path)
{
    m_selected_path = path;
    m_pix_map.load(path);
    auto width = this->width() - 20;
    auto height = this->height() - 20;
    m_pix_map = m_pix_map.scaled(width, height, Qt::KeepAspectRatio);
    ui->label->setPixmap(m_pix_map);
}

void PicShow::slotUpdatePic(const QString &path)
{
    m_selected_path = path;
    if (m_selected_path != "") {
        const auto &width = ui->gridLayout->geometry().width();
        const auto &height = ui->gridLayout->geometry().height();
        m_pix_map.load(m_selected_path);
        m_pix_map = m_pix_map.scaled(width, height, Qt::KeepAspectRatio);
        ui->label->setPixmap(m_pix_map);
    }
}

void PicShow::slotDeleteItem()
{
    m_selected_path = "";
}
