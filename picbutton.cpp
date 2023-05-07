
#include "picbutton.h"

PicButton::PicButton(QWidget *parent)
{

}

void PicButton::setIcons(const QString &normal, const QString &hover, const QString &pressed)
{
    m_normal = normal;
    m_hover = hover;
    m_pressed = pressed;

    QPixmap tmp_pixmap;
    tmp_pixmap.load(normal);
    this->resize(tmp_pixmap.size());
    this->setIcon(tmp_pixmap);
    this->setIconSize(tmp_pixmap.size()); // 保证按钮始终和原图大小一样，避免失真
}

bool PicButton::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter:
        setHoverIcon();
        break;
    case QEvent::Leave:
        setNormalIcon();
        break;
    case QEvent::MouseButtonPress:
        setPressedIcon();
        break;
    case QEvent::MouseButtonRelease:
        setHoverIcon();
        break;
    default:
        break;
    }

    return QPushButton::event(e);
}

void PicButton::setNormalIcon()
{
    QPixmap tmp_pixmap;
    tmp_pixmap.load(m_normal);
    this->setIcon(tmp_pixmap);
}

void PicButton::setHoverIcon()
{
    QPixmap tmp_pixmap;
    tmp_pixmap.load(m_hover);
    this->setIcon(tmp_pixmap);
}

void PicButton::setPressedIcon()
{
    QPixmap tmp_pixmap;
    tmp_pixmap.load(m_pressed);
    this->setIcon(tmp_pixmap);
}

