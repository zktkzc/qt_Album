#pragma once
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
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include "protreethread.h"
#include "opentreethread.h"

class slideShowDialog;

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    ProTreeWidget(QWidget* parent = nullptr);
    void addProToTree(const QString& name, const QString& path);

private:
    QSet<QString> set_path;
    QTreeWidgetItem* right_click_item;
    QTreeWidgetItem* active_item; // 当前启动项目
    QTreeWidgetItem* selected_item;
    QAction* act_import;
    QAction* act_setStart;
    QAction* act_closePro; // 关闭项目
    QAction* act_slideShow; // 轮播图播放
    QProgressDialog* m_dialog_progress;
    QProgressDialog* m_open_progressdlg;
    std::shared_ptr<ProTreeThread> m_thread_create_pro; // 使用智能指针管理线程
    std::shared_ptr<OpenTreeThread> m_thread_open_pro;
    std::shared_ptr<slideShowDialog> m_slide_show_dlg;
    QMediaPlayer *m_player;
    QMediaPlaylist *m_playlist;

public slots:
    void slotOpenPro(const QString& path);
    void slotNextShow();
    void slotPreShow();
    void slotSetMusic();
    void slotStartMusic();
    void slotStopMusic();

private slots:
    void slotItemPressed(QTreeWidgetItem* pressedItem, int column);
    void slotImport();
    void slotUpdateProgress(int file_count);
    void slotFinishProgress(int file_count);
    void slotCancleProgress();
    void slotSetActive();
    void slotClosePro();
    void slotUpdateOpenProgress(int file_count);
    void slotFinishOpenProgress(int file_count);
    void slotCancleOpenProgress();
    void slotDoubleClickItem(QTreeWidgetItem *item, int column);
    void slotSlideShow();

signals:
    void sigCancleProgress();
    void sigCancleOpenProgress();
    void sigUpdateSelected(const QString& path);
    void sigUpdatePic(const QString& path);
    void sigClearSelected();
};

#endif // PROTREEWIDGET_H
