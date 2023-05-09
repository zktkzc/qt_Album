
#include "prelistwidget.h"
#include "prelistitem.h"

PreListWidget::PreListWidget(QWidget *parent) : QListWidget(parent), m_global(0), m_last_index(17)
{
    this->setViewMode(QListWidget::IconMode);
    this->setIconSize(QSize(PREICON_SIZE, PREICON_SIZE));
    this->setSpacing(5); // 每个icon之间有5像素的间隔
    connect(this, &PreListWidget::itemPressed, this, &PreListWidget::slotItemPressed);
}

PreListWidget::~PreListWidget()
{

}

void PreListWidget::addListItem(const QString &path)
{
    QPixmap src_pixmap(path);
    src_pixmap = src_pixmap.scaled(PREICON_SIZE, PREICON_SIZE, Qt::KeepAspectRatio);
    QPixmap dist_pixmap(QSize(PREICON_SIZE, PREICON_SIZE));
    dist_pixmap.fill(QColor(220, 220, 220, 50));
    QPainter painter(&dist_pixmap);
    auto x = (dist_pixmap.width() - src_pixmap.width()) / 2;
    auto y = (dist_pixmap.height() - src_pixmap.height()) / 2;
    painter.drawPixmap(x, y, src_pixmap);
    m_global++;
    PreListItem *pItem = new PreListItem(QIcon(dist_pixmap), path, m_global, this);
    pItem->setSizeHint(QSize(PREITEM_SIZE, PREITEM_SIZE));
    this->addItem(pItem);
    m_set_items[path] = pItem;
    if (m_global == 1) {
        m_pos_origin = this->pos();
    }
}

void PreListWidget::slotUpdatePreList(QTreeWidgetItem *item)
{
    if (!item) return;
    auto *pro_item = dynamic_cast<ProTreeItem*>(item);
    auto path = pro_item->getPath();
    auto iter = m_set_items.find(path);
    if (iter != m_set_items.end()) return;
    addListItem(path);
}

void PreListWidget::slotUpdateSelectedItem(QTreeWidgetItem *item)
{
    if (!item) return;
    auto *pro_item = dynamic_cast<ProTreeItem*>(item);
    auto path = pro_item->getPath();
    auto iter = m_set_items.find(path);
    if (iter == m_set_items.end()) return;
    auto *list_item = dynamic_cast<PreListItem*>(iter.value());
    auto index = list_item->getIndex();
    if (index > 17) {
        auto pos_cur = this->pos();
        this->move(pos_cur.x() - (index - m_last_index)*100, pos_cur.y());
    } else {
        this->move(m_pos_origin);
        m_last_index = 17;
    }

    this->setCurrentItem(iter.value());
}

void PreListWidget::slotItemPressed(QListWidgetItem *item)
{
    if (QGuiApplication::mouseButtons() != Qt::LeftButton) return;
    auto *list_item = dynamic_cast<PreListItem*>(item);
    auto cur_index = list_item->getIndex();
    auto path = list_item->getPath();
    this->setCurrentItem(item);
    emit sigUpdateSelectedShow(path);
}





