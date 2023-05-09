
#include "picanimationwidget.h"

#include <QPainter>

PicAnimationWidget::PicAnimationWidget(QWidget *parent)
    : QWidget{parent}, m_factor(0.0), m_cur_item(nullptr), m_b_start(false)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &PicAnimationWidget::slotTimeOut);
}

PicAnimationWidget::~PicAnimationWidget()
{

}

void PicAnimationWidget::setPixmap(QTreeWidgetItem *item)
{
    if (!item) return;
    auto *tree_item = dynamic_cast<ProTreeItem*>(item);
    auto path = tree_item->getPath();
    m_pixmap1.load(path);
    m_cur_item = tree_item;

    if (m_map_items.find(path) == m_map_items.end()) {
        m_map_items[path] = tree_item;
        // 发送更新列表逻辑
        emit sigUpdatePreList(tree_item);
    }

    emit sigUpdateSelectedItem(item);

    auto *next_item = tree_item->getNextItem();
    if (!next_item) return;
    auto next_path = next_item->getPath();
    m_pixmap2.load(next_path);
    if (m_map_items.find(next_path) == m_map_items.end()) {
        m_map_items[next_path] = next_item;
        // 发送更新列表逻辑
        emit sigUpdatePreList(next_item);
    }
}

void PicAnimationWidget::start()
{
    m_factor = 0;
    m_timer->start(25);
    m_b_start = true;
}

void PicAnimationWidget::stop()
{
    m_timer->stop();
    m_factor = 0;
    m_b_start = false;
}

void PicAnimationWidget::paintEvent(QPaintEvent *event)
{
    if (m_pixmap1.isNull()) return;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // 防锯齿
    QRect rect = geometry();
    int w = rect.width();
    int h = rect.height();
    m_pixmap1 = m_pixmap1.scaled(w, h, Qt::KeepAspectRatio); // 将图片拉伸到矩形区域
    int alpha = 255 * (1.0f - m_factor);
    QPixmap alphaPixmap(m_pixmap1.size());
    alphaPixmap.fill(Qt::transparent); // 设置为透明的

    QPainter p1(&alphaPixmap);
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0, 0, m_pixmap1);
    p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p1.fillRect(alphaPixmap.rect(), QColor(0,0,0,alpha)); // 画一个黑色的遮罩，渐隐
    p1.end();

    int x = (w - m_pixmap1.width()) / 2;
    int y = (h - m_pixmap1.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap);

    if (m_pixmap2.isNull()) return;
    m_pixmap2 = m_pixmap2.scaled(w, h, Qt::KeepAspectRatio);
    alpha = 255 * m_factor;
    QPixmap alphaPixmap2(m_pixmap2.size());
    alphaPixmap2.fill(Qt::transparent);
    QPainter p2(&alphaPixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_Source);
    p2.drawPixmap(0, 0, m_pixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p2.fillRect(alphaPixmap2.rect(), QColor(0,0,0, alpha)); // 渐显
    p2.end();
    x = (w - m_pixmap2.width()) / 2;
    y = (h - m_pixmap2.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap2);
}

void PicAnimationWidget::slotTimeOut()
{
    if (!m_cur_item) {
        stop();
        update();
        return;
    }

    m_factor += 0.01;
    if (m_factor >= 1) {
        m_factor = 0;
        auto *cur_pro_item = dynamic_cast<ProTreeItem*>(m_cur_item);
        auto *next_pro_item = cur_pro_item->getNextItem();
        if (!next_pro_item) {
            stop();
            update();
            return;
        }

        setPixmap(next_pro_item);
        update();
        return;
    }

    update();
}

