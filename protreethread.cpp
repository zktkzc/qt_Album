
#include "protreethread.h"
#include "protreeitem.h"
#include "const.h"

///
/// \brief ProTreeThread::ProTreeThread
/// \param src_path       源路径
/// \param dist_path      目标路径
/// \param parent_item    父节点
/// \param file_count     文件数量
/// \param self           所在的QTreeWidget
/// \param root           根节点
/// \param parent         父窗口
///
ProTreeThread::ProTreeThread(const QString& src_path, const QString& dist_path, QTreeWidgetItem* parent_item, int file_count, QTreeWidget* self,
                             QTreeWidgetItem* root, QObject* parent)
    : QThread(parent), m_src_path(src_path), m_dist_path(dist_path), m_file_count(file_count), m_parent_item(parent_item), m_self(self), m_root(root),
    mb_stop(false)
{

}

ProTreeThread::~ProTreeThread()
{

}

void ProTreeThread::run()
{
    createProTree(m_src_path, m_dist_path, m_parent_item, m_file_count, m_self, m_root);
    if (mb_stop) {
        auto path = dynamic_cast<ProTreeItem*>(m_root)->getPath();
        auto index = m_self->indexOfTopLevelItem(m_root);
        delete m_self->takeTopLevelItem(index); // 删除节点
        QDir dir(path);
        dir.removeRecursively(); // 删除目录
        return;
    }

    emit sigFinishProgress(m_file_count);
}

void ProTreeThread::createProTree(const QString &src_path, const QString &dist_path, QTreeWidgetItem *parent_item, int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *pre_item)
{
    if (mb_stop) return;
    bool needCopy = true; // 是否需要拷贝，默认都是从源路径拷贝到目标路径
    if (src_path == dist_path) needCopy = false;
    QDir import_dir(src_path);
    QStringList nameFilters;
    import_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    import_dir.setSorting(QDir::Name);
    QFileInfoList list = import_dir.entryInfoList();
    for (int i = 0; i < list.size(); i++) {
        if (mb_stop) return;
        QFileInfo fileInfo = list.at(i);
        bool b_isDir = fileInfo.isDir();
        if (b_isDir) {
            if (mb_stop) return;

            file_count++;
            emit sigUpdateProgress(file_count);

            QDir dist_dir(dist_path);
            QString sub_dist_path = dist_dir.absoluteFilePath(fileInfo.fileName());
            QDir sub_dist_dir(sub_dist_path);
            if (!sub_dist_dir.exists()) {
                bool ok = sub_dist_dir.mkpath(sub_dist_path);
                if (!ok) continue;
            }

            auto* item = new ProTreeItem(parent_item, fileInfo.fileName(), sub_dist_path, root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, sub_dist_path);

            createProTree(fileInfo.absoluteFilePath(), sub_dist_path, item, file_count, self, root, pre_item);

        } else {
            if (mb_stop) return;

            const QString& suffix = fileInfo.completeSuffix();
            if (suffix != "png" && suffix != "jpeg" && suffix != "jpg") continue;

            file_count++;
            emit sigUpdateProgress(file_count);
            if (!needCopy) continue;
            QDir dist_dir(dist_path);
            QString dist_file_path = dist_dir.absoluteFilePath(fileInfo.fileName());
            if (!QFile::copy(fileInfo.absoluteFilePath(), dist_file_path)) continue;
            auto* item = new ProTreeItem(parent_item, fileInfo.fileName(), dist_file_path, root, TreeItemPic);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, dist_file_path);

            if (pre_item) {
                auto* pre_proitem = dynamic_cast<ProTreeItem*>(pre_item);
                pre_proitem->setNextItem(item);
            }

            item->setPreItem(pre_item);
            pre_item = item;
        }
    }
}

void ProTreeThread::slotCancleProgress()
{
    this->mb_stop = true;
}

