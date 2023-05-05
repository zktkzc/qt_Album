
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QWidget>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void SlotCreatePro(bool);
    void SlotOpenPro(bool);

signals:
    void sigOpenPro(const QString path);

private:
    Ui::MainWindow *ui;
    QWidget* proTree;
};

#endif // MAINWINDOW_H
