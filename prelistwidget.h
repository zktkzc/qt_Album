
#ifndef PRELISTWIDGET_H
#define PRELISTWIDGET_H

#include <QListWidget>
#include <QTreeWidgetItem>
#include <QPixmap>
#include <QPainter>
#include <QGuiApplication>
#include <protreeitem.h>
#include <const.h>

class PreListWidget : public QListWidget
{
    Q_OBJECT

public:
    PreListWidget(QWidget *parent = nullptr);
    ~PreListWidget();

private:
    void addListItem(const QString& path);

public slots:
    void slotUpdatePreList(QTreeWidgetItem* item);
    void slotUpdateSelectedItem(QTreeWidgetItem* item);
    void slotItemPressed(QListWidgetItem* item);

private:
    QMap<QString, QListWidgetItem*> m_set_items;
    int m_global;
    QPoint m_pos_origin;
    int m_last_index;

signals:
    void sigUpdateSelectedShow(QString path);
};

#endif // PRELISTWIDGET_H
