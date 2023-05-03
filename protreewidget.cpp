
#include "protreewidget.h"
#include "protreeitem.h"
#include "const.h"

ProTreeWidget::ProTreeWidget(QWidget* parent)
{
    // 隐藏表头
    this->header()->hide();
    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::slotItemPressed);

    // 创建动作
    act_import = new QAction(QIcon(":/icon/import.png"), tr("导入文件"), this);
    act_setStart = new QAction(QIcon(":/icon/core.png"), tr("设置活动项目"), this);
    act_closePro = new QAction(QIcon(":/icon/close.png"), tr("关闭项目"), this);
    act_slideShow = new QAction(QIcon(":/icon/slideshow.png"), tr("轮播图播放"), this);

    connect(act_import, &QAction::triggered, this, &ProTreeWidget::slotImport);
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

void ProTreeWidget::slotItemPressed(QTreeWidgetItem *pressedItem, int column)
{
    // 右键点击item
    if (QGuiApplication::mouseButtons() == Qt::RightButton) {
        QMenu menu(this);
        int itemType = pressedItem->type();
        if (itemType == TreeItemPro) {
            right_click_item = pressedItem;
            menu.addAction(act_import);
            menu.addAction(act_setStart);
            menu.addAction(act_closePro);
            menu.addAction(act_slideShow);
            menu.exec(QCursor::pos());
        }
    }
}

void ProTreeWidget::slotImport()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("选择导入的文件夹"));
    QString path = "";
    if (!right_click_item) {
        qDebug() << "right_click_item is empty.";
        return;
    }

    path = dynamic_cast<ProTreeItem*>(right_click_item)->getPath();
    fileDialog.setDirectory(path); // 设置默认打开路径
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (fileDialog.exec()) fileNames = fileDialog.selectedFiles();
    if (fileNames.length() <= 0) return;

    QString import_path = fileNames.at(0);
    int file_count = 0;
    // 创建模态对话框
    m_dialog_progress = new QProgressDialog(this);

    // 创建线程，线程传递的参数都是右值，需要使用ref函数进行转换
    m_thread_create_pro = std::make_shared<ProTreeThread>(std::ref(import_path), std::ref(path), right_click_item, file_count, this, right_click_item, nullptr);
    m_thread_create_pro->start(); // 启动线程

    connect(m_thread_create_pro.get(), &ProTreeThread::sigUpdateProgress, this, &ProTreeWidget::slotUpdateProgress);
    connect(m_thread_create_pro.get(), &ProTreeThread::sigFinishProgress, this, &ProTreeWidget::slotFinishProgress);
    connect(m_dialog_progress, &QProgressDialog::canceled, this, &ProTreeWidget::slotCancleProgress);
    connect(this, &ProTreeWidget::sigCancleProgress, m_thread_create_pro.get(), &ProTreeThread::slotCancleProgress);

    // 初始化对话框
    m_dialog_progress->setWindowTitle("Please wait...");
    m_dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    m_dialog_progress->setRange(0, PROGRESS_WIDTH); // 设置波动范围
    m_dialog_progress->exec(); // 模态显示对话框
}

void ProTreeWidget::slotUpdateProgress(int file_count)
{
    if (m_dialog_progress) return;
    if (file_count >= PROGRESS_MAX) m_dialog_progress->setValue(file_count%PROGRESS_MAX);
    else m_dialog_progress->setValue(file_count);
}

void ProTreeWidget::slotFinishProgress(int file_count)
{
    m_dialog_progress->setValue(PROGRESS_MAX);
    m_dialog_progress->deleteLater();
}

void ProTreeWidget::slotCancleProgress()
{
    emit sigCancleProgress();
    delete m_dialog_progress;
    m_dialog_progress = nullptr;
}

