
#ifndef PRELISTWIDGET_H
#define PRELISTWIDGET_H

#include <QListWidget>
#include <QTreeWidgetItem>
#include <QPixmap>
#include <QPainter>
#include <protreeitem.h>
#include <const.h>

class PreListWidget : public QListWidget
{
public:
    PreListWidget(QWidget *parent = nullptr);
    ~PreListWidget();

private:
    void addListItem(const QString& path);

public slots:
    void slotUpdatePreList(QTreeWidgetItem* item);
    void slotUpdateSelectedItem(QTreeWidgetItem* item);

private:
    QMap<QString, QListWidgetItem*> m_set_items;
    int m_global;
    QPoint m_pos_origin;
    int m_last_index;
};

#endif // PRELISTWIDGET_H
