
#ifndef OPENTREETHREAD_H
#define OPENTREETHREAD_H

#include <QThread>
#include <QTreeWidget>
#include <QDir>
#include <QFileInfoList>

class OpenTreeThread : public QThread
{
    Q_OBJECT

public:
    explicit OpenTreeThread(const QString& src_path, int file_count, QTreeWidget* self, QObject *parent = nullptr);
    void openProTree(const QString& src_path, int& file_count, QTreeWidget* self);

protected:
    void run();

private:
    void recursiveProTree(const QString& src_path, int& file_count, QTreeWidget* self, QTreeWidgetItem* root, QTreeWidgetItem* parent,
                          QTreeWidgetItem* pre_item);

private:
    QString m_src_path;
    int m_file_count;
    QTreeWidget* m_self;
    bool m_bstop;
    QTreeWidgetItem* m_root;

public slots:
    void slotCancleOpenProgress();

signals:
    void sigFinishProgress(int);
    void sigUpdateProgress(int);
};

#endif // OPENTREETHREAD_H
