
#ifndef PICSTATEBTN_H
#define PICSTATEBTN_H

#include <QPushButton>
#include <QEvent>
#include <QPixmap>


class PicStateBtn : public QPushButton
{
public:
    PicStateBtn(QWidget* parent = nullptr);
    void setIcons(const QString& normal, const QString& hover, const QString& pressed, const QString& normal_2, const QString& hover_2,
                  const QString& pressed_2);
    virtual ~PicStateBtn() {};

private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressIcon();
    void setNormal2Icon();
    void setHover2Icon();
    void setPress2Icon();

protected:
    bool event(QEvent* event) override;

private:
    QString m_normal;
    QString m_hover;
    QString m_pressed;
    QString m_normal2;
    QString m_hover2;
    QString m_pressed2;
    int m_cur_state;

public slots:
    void slotStart();
    void slotStop();
};

#endif // PICSTATEBTN_H
