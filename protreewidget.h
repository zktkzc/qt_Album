
#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include <QWidget>
#include <QDir>
#include <QSet>
#include <QIcon>
#include <QDebug>
#include <QHeaderView>
#include <QGuiApplication>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QProgressDialog>
#include "protreethread.h"

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    ProTreeWidget(QWidget* parent = nullptr);
    void addProToTree(const QString& name, const QString& path);

private:
    QSet<QString> set_path;
    QTreeWidgetItem* right_click_item;
    QAction* act_import;
    QAction* act_setStart;
    QAction* act_closePro; // 关闭项目
    QAction* act_slideShow; // 轮播图播放
    QProgressDialog* m_dialog_progress;
    std::shared_ptr<ProTreeThread> m_thread_create_pro; // 使用智能指针管理线程

private slots:
    void slotItemPressed(QTreeWidgetItem* pressedItem, int column);
    void slotImport();
    void slotUpdateProgress(int file_count);
    void slotFinishProgress(int file_count);
    void slotCancleProgress();

signals:
    void sigCancleProgress();
};

#endif // PROTREEWIDGET_H
