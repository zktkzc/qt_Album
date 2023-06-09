#ifndef PROTREE_H
#define PROTREE_H

#include <QDialog>
#include <QWidget>
#include <QTreeWidget>

namespace Ui {
class ProTree;
}

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    ~ProTree();
    QTreeWidget* getTreeWidget();

public slots:
    void addProToTree(const QString name, const QString path);

private:
    Ui::ProTree *ui;
};

#endif // PROTREE_H
