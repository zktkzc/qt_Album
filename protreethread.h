
#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QDir>
#include <QFileInfoList>

class ProTreeThread : public QThread
{
    Q_OBJECT

public:
    ProTreeThread(const QString& src_path, const QString& dist_path, QTreeWidgetItem* parent_item, int file_count, QTreeWidget* self,
                  QTreeWidgetItem* root, QObject* parent = nullptr);
    ~ProTreeThread();

protected:
    void run();

private:
    QString m_src_path;
    QString m_dist_path;
    int m_file_count;
    QTreeWidgetItem* m_parent_item;
    QTreeWidget* m_self;
    QTreeWidgetItem* m_root;
    bool mb_stop;

private:
    void createProTree(const QString& src_path, const QString& dist_path, QTreeWidgetItem* parent_item, int &file_count, QTreeWidget* self,
                       QTreeWidgetItem* root, QTreeWidgetItem* pre_item = nullptr);

public slots:
    void slotCancleProgress();

signals:
    void sigUpdateProgress(int);
    void sigFinishProgress(int);
};

#endif // PROTREETHREAD_H
