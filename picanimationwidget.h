
#ifndef PICANIMATIONWIDGET_H
#define PICANIMATIONWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QPaintEvent>
#include <protreeitem.h>

class PicAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimationWidget(QWidget *parent = nullptr);
    ~PicAnimationWidget();
    void setPixmap(QTreeWidgetItem* item);
    void start();
    void stop();
    void slideNext();
    void slidePre();

protected:
    void paintEvent(QPaintEvent *event);

private:
    void updateSelectedPixmap(QTreeWidgetItem* item);

private:
    float m_factor;
    QTimer* m_timer;
    QPixmap m_pixmap1;
    QPixmap m_pixmap2;
    QTreeWidgetItem* m_cur_item;
    QMap<QString, QTreeWidgetItem*> m_map_items;
    bool m_b_start;

signals:
    void sigUpdatePreList(QTreeWidgetItem* item);
    void sigUpdateSelectedItem(QTreeWidgetItem* item);
    void sigStart();
    void sigStop();
    void sigStartMusic();
    void sigStopMusic();

public slots:
    void slotUpdateSelectedShow(QString path);
    void slotStartOrStop();

private slots:
    void slotTimeOut();

};

#endif // PICANIMATIONWIDGET_H
