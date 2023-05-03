
#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidgetItem>
#include <QTreeWidget>

class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem(QTreeWidget* view, const QString& name, const QString& path, int type = Type);
    ProTreeItem(QTreeWidgetItem* parent, const QString& name, const QString& path, QTreeWidgetItem* root, int type = Type);
    const QString& getPath();
    QTreeWidgetItem* getRoot();
    void setPreItem(QTreeWidgetItem* item);
    void setNextItem(QTreeWidgetItem* item);
    ProTreeItem* getPreItem();
    ProTreeItem* getNextItem();
    ProTreeItem* getLastPicChild();
    ProTreeItem* getFirstPicChild();

private:
    QString m_path;
    QString m_name;
    QTreeWidgetItem* m_root;
    QTreeWidgetItem* m_pre_item;
    QTreeWidgetItem* m_next_item;
};

#endif // PROTREEITEM_H
