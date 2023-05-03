
#include "protreeitem.h"

ProTreeItem::ProTreeItem(QTreeWidget* view, const QString& name, const QString& path, int type)
    : QTreeWidgetItem(view, type), m_path(path), m_name(name), m_root(this), m_pre_item(nullptr), m_next_item(nullptr)
{

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type)
    : QTreeWidgetItem(parent, type), m_path(path), m_name(name), m_root(root), m_pre_item(nullptr), m_next_item(nullptr)
{

}

const QString &ProTreeItem::getPath()
{
    return m_path;
}

QTreeWidgetItem *ProTreeItem::getRoot()
{
    return m_root;
}

void ProTreeItem::setPreItem(QTreeWidgetItem *item)
{
    m_pre_item = item;
}

void ProTreeItem::setNextItem(QTreeWidgetItem *item)
{
    m_next_item = item;
}

ProTreeItem *ProTreeItem::getPreItem()
{
    return dynamic_cast<ProTreeItem*>(m_pre_item);
}

ProTreeItem *ProTreeItem::getNextItem()
{
    return dynamic_cast<ProTreeItem*>(m_next_item);
}

ProTreeItem *ProTreeItem::getLastPicChild()
{

}

ProTreeItem *ProTreeItem::getFirstPicChild()
{

}

