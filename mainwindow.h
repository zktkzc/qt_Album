
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QWidget>
#include <QFileDialog>
#include <QResizeEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void SlotCreatePro(bool);
    void SlotOpenPro(bool);

signals:
    void sigOpenPro(const QString path);

private:
    Ui::MainWindow *ui;
    QWidget* proTree;
    QWidget* m_picShow;
};

#endif // MAINWINDOW_H
