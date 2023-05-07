
#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QEvent>

class PicButton : public QPushButton
{
public:
    PicButton(QWidget* parent = nullptr);
    void setIcons(const QString& normal, const QString& hover, const QString& pressed);

protected:
    bool event(QEvent* e) override;

private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressedIcon();

private:
    QString m_normal;
    QString m_hover;
    QString m_pressed;
};

#endif // PICBUTTON_H
