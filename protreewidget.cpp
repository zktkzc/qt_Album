
#include "protreewidget.h"
#include "protreeitem.h"
#include "const.h"
#include "removeprodialog.h"
#include "slideshowdialog.h"

ProTreeWidget::ProTreeWidget(QWidget* parent)
    : QTreeWidget(parent), active_item(nullptr), right_click_item(nullptr), m_dialog_progress(nullptr), selected_item(nullptr),
    m_thread_create_pro(nullptr), m_thread_open_pro(nullptr), m_open_progressdlg(nullptr)
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
    connect(act_setStart, &QAction::triggered, this, &ProTreeWidget::slotSetActive);
    connect(act_closePro, &QAction::triggered, this, &ProTreeWidget::slotClosePro);
    connect(this, &ProTreeWidget::itemDoubleClicked, this, &ProTreeWidget::slotDoubleClickItem);
    connect(act_slideShow, &QAction::triggered, this, &ProTreeWidget::slotSlideShow);

    m_player = new QMediaPlayer(this);
    m_playlist = new QMediaPlaylist(this);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_player->setPlaylist(m_playlist);
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

void ProTreeWidget::slotOpenPro(const QString &path)
{
    if (set_path.find(path) != set_path.end()) return;
    set_path.insert(path);
    int file_count = 0;
    QDir pro_dir(path);
    const QString proName = pro_dir.dirName();
    m_thread_open_pro = std::make_shared<OpenTreeThread>(path, file_count, this, nullptr);

    m_open_progressdlg = new QProgressDialog(this);

    connect(m_thread_open_pro.get(), &OpenTreeThread::sigUpdateProgress, this, &ProTreeWidget::slotUpdateOpenProgress);
    connect(m_thread_open_pro.get(), &OpenTreeThread::sigFinishProgress, this, &ProTreeWidget::slotFinishOpenProgress);
    connect(m_open_progressdlg, &QProgressDialog::canceled, this, &ProTreeWidget::slotCancleOpenProgress);
    connect(this, &ProTreeWidget::sigCancleOpenProgress, m_thread_open_pro.get(), &OpenTreeThread::slotCancleOpenProgress);

    m_thread_open_pro->start();

    // 初始化对话框
    m_open_progressdlg->setWindowTitle("Please wait...");
    m_open_progressdlg->setFixedWidth(PROGRESS_WIDTH);
    m_open_progressdlg->setRange(0, PROGRESS_WIDTH); // 设置波动范围
    m_open_progressdlg->exec(); // 模态显示对话框
}

void ProTreeWidget::slotNextShow()
{
    if (!selected_item) return;
    auto *curItem = dynamic_cast<ProTreeItem*>(selected_item)->getNextItem();
    if (!curItem) return;
    emit sigUpdatePic(curItem->getPath());
    selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::slotPreShow()
{
    if (!selected_item) return;
    auto *curItem = dynamic_cast<ProTreeItem*>(selected_item)->getPreItem();
    if (!curItem) return;
    emit sigUpdatePic(curItem->getPath());
    selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::slotSetMusic()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::ExistingFiles);
    file_dialog.setWindowTitle(tr("请选择音频文件"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setNameFilter("(*.mp3)");
    QStringList fileNames;
    if (file_dialog.exec()) fileNames = file_dialog.selectedFiles();
    else return;
    if (fileNames.length() <= 0) return;
    m_playlist->clear();
    for (auto filename : fileNames) m_playlist->addMedia(QUrl::fromLocalFile(filename));
    if (m_player->state() != QMediaPlayer::PlayingState) m_playlist->setCurrentIndex(0);
}

void ProTreeWidget::slotStartMusic()
{
    m_player->play();
}

void ProTreeWidget::slotStopMusic()
{
    m_player->stop();
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

void ProTreeWidget::slotSetActive()
{
    if (!right_click_item) return;

    QFont nullFont;
    nullFont.setBold(false);

    if (active_item) active_item->setFont(0, nullFont);
    active_item = right_click_item;
    nullFont.setBold(true);
    active_item->setFont(0, nullFont);
}

void ProTreeWidget::slotClosePro()
{
    RemoveProDialog remove_pro_dialog;
    auto res = remove_pro_dialog.exec();
    if (res != QDialog::Accepted) return;
    bool b_remove = remove_pro_dialog.isRemoved();
    auto index_right_btn = this->indexOfTopLevelItem(right_click_item);
    auto *pro_tree_item = dynamic_cast<ProTreeItem*>(right_click_item);
    auto *selectedItem = dynamic_cast<ProTreeItem*>(selected_item);
    auto delete_path = pro_tree_item->getPath();
    set_path.remove(delete_path);
    if (b_remove) {
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively(); // 删除目录
    }
    if (pro_tree_item == active_item) active_item = nullptr;
    if (selectedItem && pro_tree_item == selectedItem->getRoot()) {
        selectedItem = nullptr;
        selected_item = nullptr;
        emit sigClearSelected();
    }
    delete this->takeTopLevelItem(index_right_btn);
    right_click_item = nullptr;
}

void ProTreeWidget::slotUpdateOpenProgress(int file_count)
{
    if (m_open_progressdlg) return;
    if (file_count >= PROGRESS_MAX) m_open_progressdlg->setValue(file_count%PROGRESS_MAX);
    else m_open_progressdlg->setValue(file_count);
}

void ProTreeWidget::slotFinishOpenProgress(int file_count)
{
    m_open_progressdlg->setValue(PROGRESS_MAX);
    m_open_progressdlg->deleteLater();
}

void ProTreeWidget::slotCancleOpenProgress()
{
    emit sigCancleProgress();
    delete m_open_progressdlg;
    m_open_progressdlg = nullptr;
}

void ProTreeWidget::slotDoubleClickItem(QTreeWidgetItem *item, int column)
{
    if (QGuiApplication::mouseButtons() == Qt::LeftButton) {
        auto *tree_doubleItem = dynamic_cast<ProTreeItem*>(item);
        if (!tree_doubleItem) return;

        int item_type = tree_doubleItem->type();
        if (item_type == TreeItemPic) {
            emit sigUpdateSelected(tree_doubleItem->getPath());
            selected_item = item;
        }
    }
}

void ProTreeWidget::slotSlideShow()
{
    if (!right_click_item) return;
    auto *right_pro_item = dynamic_cast<ProTreeItem*>(right_click_item);
    auto *last_child_item = right_pro_item->getLastPicChild();
    if (!last_child_item) return;
    auto *first_child_item = right_pro_item->getFirstPicChild();
    if (!first_child_item) return;
    qDebug() << "first_child_item is " << first_child_item->getPath();
    qDebug() << "last_child_item is " << last_child_item->getPath();
    m_slide_show_dlg = std::make_shared<slideShowDialog>(this, first_child_item, last_child_item);
    m_slide_show_dlg->setModal(true);
    m_slide_show_dlg->showMaximized();
}

