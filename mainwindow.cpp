
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wizard.h"
#include "protree.h"
#include "protreewidget.h"
#include "picshow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setMinimumSize(1629, 869);
    ui->setupUi(this);
    // 创建菜单
    QMenu* menu_file = menuBar()->addMenu(tr("文件(&F)"));

    // 创建项目动作
    QAction* act_create_pro = new QAction(QIcon(":/icon/createpro.png"), tr("创建项目"), this);
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    menu_file->addAction(act_create_pro);

    // 打开项目动作
    QAction* act_open_pro = new QAction(QIcon(":/icon/openpro.png"), tr("打开项目"), this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menu_file->addAction(act_open_pro);

    // 创建设置菜单
    QMenu* menu_set = menuBar()->addMenu(tr("设置(&S)"));

    // 设置背景音乐
    QAction* act_music = new QAction(QIcon(":/icon/music.png"), tr("背景音乐"), this);
    act_music->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    menu_set->addAction(act_music);

    // 连接信号和槽
    connect(act_create_pro, &QAction::triggered, this, &MainWindow::SlotCreatePro);
    connect(act_open_pro, &QAction::triggered, this, &MainWindow::SlotOpenPro);
    proTree = new ProTree();
    ui->proLayout->addWidget(proTree, 0);
    auto *tree_widget = dynamic_cast<ProTree*>(proTree)->getTreeWidget();
    auto *pro_tree_widget = dynamic_cast<ProTreeWidget*>(tree_widget);
    connect(this, &MainWindow::sigOpenPro, pro_tree_widget, &ProTreeWidget::slotOpenPro);

    m_picShow = new PicShow();
    ui->picLayout->addWidget(m_picShow);

    auto *pro_pic_show = dynamic_cast<PicShow*>(m_picShow);
    connect(pro_tree_widget, &ProTreeWidget::sigUpdateSelected, pro_pic_show, &PicShow::slotSelectedItem);

    connect(pro_pic_show, &PicShow::sigNextClicked, pro_tree_widget, &ProTreeWidget::slotNextShow);
    connect(pro_pic_show, &PicShow::sigPreClicked, pro_tree_widget, &ProTreeWidget::slotPreShow);
    connect(pro_tree_widget, &ProTreeWidget::sigUpdatePic, pro_pic_show, &PicShow::slotUpdatePic);
    connect(pro_tree_widget, &ProTreeWidget::sigClearSelected, pro_pic_show, &PicShow::slotDeleteItem);
    connect(act_music, &QAction::triggered, pro_tree_widget, &ProTreeWidget::slotSetMusic);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    auto *pro_pic_show = dynamic_cast<PicShow*>(m_picShow);
    pro_pic_show->reloadPic();
    QMainWindow::resizeEvent(event);
}

void MainWindow::SlotCreatePro(bool)
{
    qDebug() << "slot create pro triggered";
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    auto *page = wizard.page(0);
    page->setTitle(tr("设置项目配置"));
    // 连接信号和槽，把项目配置传回来
    connect(&wizard, &Wizard::sigProSettings, dynamic_cast<ProTree*>(proTree), &ProTree::addProToTree);
    wizard.show();
    wizard.exec();
    // 断开所有信号
    disconnect();
}

void MainWindow::SlotOpenPro(bool)
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("请选择打开的文件夹"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (file_dialog.exec()) fileNames = file_dialog.selectedFiles();
    if (fileNames.length() <= 0) return;
    QString import_path = fileNames.at(0);
    emit sigOpenPro(import_path);
}


