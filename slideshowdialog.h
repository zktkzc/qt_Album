#ifndef SLIDESHOWDIALOG_H
#define SLIDESHOWDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class slideShowDialog;
}

class slideShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit slideShowDialog(QWidget *parent = nullptr, QTreeWidgetItem* first_item = nullptr, QTreeWidgetItem* last_item = nullptr);
    ~slideShowDialog();

private:
    Ui::slideShowDialog *ui;
    QTreeWidgetItem* m_first_item;
    QTreeWidgetItem* m_last_item;
};

#endif // SLIDESHOWDIALOG_H
