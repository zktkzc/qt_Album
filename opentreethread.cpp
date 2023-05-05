
#include "opentreethread.h"
#include "protreeitem.h"
#include "const.h"

OpenTreeThread::OpenTreeThread(const QString &src_path, int file_count, QTreeWidget *self, QObject *parent)
    : QThread(parent), m_bstop(false), m_src_path(src_path), m_file_count(file_count), m_self(self), m_root(nullptr)
{

}

void OpenTreeThread::openProTree(const QString &src_path, int &file_count, QTreeWidget *self)
{
    QDir src_dir(src_path);
    auto name = src_dir.dirName();
    auto *item = new ProTreeItem(self, name, src_path, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, src_path);
    m_root = item;
    recursiveProTree(src_path, file_count, self, m_root, item, nullptr);
}

void OpenTreeThread::run()
{
    openProTree(m_src_path, m_file_count, m_self);
    if (m_bstop) {
        auto path = dynamic_cast<ProTreeItem*>(m_root)->getPath();
        auto index = m_self->indexOfTopLevelItem(m_root);
        delete m_self->takeTopLevelItem(index); // 删除节点
        QDir dir(path);
        dir.removeRecursively(); // 删除目录
        return;
    }

    emit sigFinishProgress(m_file_count);
}

void OpenTreeThread::recursiveProTree(const QString &src_path, int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *pre_item)
{
    QDir src_dir(src_path);
    src_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    src_dir.setSorting(QDir::Name);
    QFileInfoList list = src_dir.entryInfoList();
    for (int i = 0; i < list.size(); i++) {
        if (m_bstop) return;
        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir();
        if (bIsDir) {
            if (m_bstop) return;
            file_count++;
            emit sigUpdateProgress(file_count);
            auto *item = new ProTreeItem(m_root, fileInfo.fileName(), fileInfo.filePath(), m_root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());
            recursiveProTree(fileInfo.absoluteFilePath(), file_count, self, m_root, item, pre_item);
        } else {
            if (m_bstop) return;
            const QString& suffix = fileInfo.completeSuffix();
            if (suffix != "png" && suffix != "jpeg" && suffix != "jpg") continue;
            file_count++;
            emit sigUpdateProgress(file_count);
            auto *item = new ProTreeItem(m_root, fileInfo.fileName(), fileInfo.filePath(), m_root, TreeItemPic);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());

            if (pre_item) {
                auto * pre_proitem = dynamic_cast<ProTreeItem*>(pre_item);
                pre_proitem->setNextItem(item);
            }

            item->setPreItem(pre_item);
            pre_item = item;
        }
    }

    emit sigFinishProgress(file_count);
}

void OpenTreeThread::slotCancleOpenProgress()
{
    this->m_bstop = true;
}
