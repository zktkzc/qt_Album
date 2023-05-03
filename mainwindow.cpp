
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotCreatePro(bool)
{
    qDebug() << "slot create pro triggered";
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    auto *page = wizard.page(0);
    page->setTitle(tr("设置项目配置"));
    // TODO 连接信号和槽，把项目配置传回来
    wizard.show();
    wizard.exec();
    // TODO 断开所有信号
}


