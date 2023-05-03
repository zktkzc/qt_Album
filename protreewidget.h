
#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include <QWidget>
#include <QDir>
#include <QSet>
#include <QIcon>
#include <QDebug>
#include <QHeaderView>

class ProTreeWidget : public QTreeWidget
{
public:
    ProTreeWidget(QWidget* parent = nullptr);
    void addProToTree(const QString& name, const QString& path);

private:
    QSet<QString> set_path;
};

#endif // PROTREEWIDGET_H
