
#include "protreewidget.h"
#include "protreeitem.h"
#include "const.h"

ProTreeWidget::ProTreeWidget(QWidget* parent)
{
    // 隐藏表头
    this->header()->hide();
}

void ProTreeWidget::addProToTree(const QString &name, const QString &path)
{
    qDebug() << tr("项目名称：") << name << tr("，项目路径：") << path << Qt::endl;
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);
    if (set_path.find(file_path) != set_path.end()) {
        return;
    }

    // 创建文件夹
    QDir pro_dir(file_path);
    if (!pro_dir.exists()) {
        bool enable = pro_dir.mkpath(file_path);
        if (!enable) {
            return;
        }
    }

    set_path.insert(file_path);
    auto *item = new ProTreeItem(this, name, file_path, TreeItemPro);
    item->setData(0,Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, file_path);
    this->addTopLevelItem(item);
}

