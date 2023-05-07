#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();
    void reloadPic();

protected:
    bool event(QEvent *event) override;

private:
    void showPreNextBtns(bool b_show);

private:
    Ui::PicShow *ui;
    QPropertyAnimation *m_animation_show_pre;
    QPropertyAnimation *m_animation_show_next;
    bool m_b_btnVisible;
    QString m_selected_path;
    QPixmap m_pix_map;

public slots:
    void slotSelectedItem(const QString& path);
    void slotUpdatePic(const QString& path);
    void slotDeleteItem();

signals:
    void sigNextClicked();
    void sigPreClicked();
};

#endif // PICSHOW_H
