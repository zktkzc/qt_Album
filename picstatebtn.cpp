
#include "picstatebtn.h"
#include "const.h"

PicStateBtn::PicStateBtn(QWidget *parent) : QPushButton(parent)
{

}

void PicStateBtn::setIcons(const QString &normal, const QString &hover, const QString &pressed, const QString &normal_2, const QString &hover_2, const QString &pressed_2)
{
    m_normal = normal;
    m_hover = hover;
    m_pressed = pressed;
    m_normal2 = normal_2;
    m_hover2 = hover_2;
    m_pressed2 = pressed_2;

    QPixmap tmpPixMap;
    tmpPixMap.load(normal);
    this->resize(tmpPixMap.size());
    this->setIcon(tmpPixMap);
    this->setIconSize(tmpPixMap.size());
    m_cur_state = PicBtnStateNormal;
}

void PicStateBtn::setNormalIcon()
{
    QPixmap tmpPixMap;
    tmpPixMap.load(m_normal);
    this->setIcon(tmpPixMap);
    m_cur_state = PicBtnStateNormal;
}

void PicStateBtn::setHoverIcon()
{
    QPixmap tmpPixMap;
    tmpPixMap.load(m_hover);
    this->setIcon(tmpPixMap);
    m_cur_state = PicBtnStateHover;
}

void PicStateBtn::setPressIcon()
{
    QPixmap tmpPixMap;
    tmpPixMap.load(m_pressed);
    this->setIcon(tmpPixMap);
    m_cur_state = PicBtnStatePressed;
}

void PicStateBtn::setNormal2Icon()
{
    QPixmap tmpPixMap;
    tmpPixMap.load(m_normal2);
    this->setIcon(tmpPixMap);
    m_cur_state = PicBtnState2Normal;
}

void PicStateBtn::setHover2Icon()
{
    QPixmap tmpPixMap;
    tmpPixMap.load(m_hover2);
    this->setIcon(tmpPixMap);
    m_cur_state = PicBtnState2Hover;
}

void PicStateBtn::setPress2Icon()
{
    QPixmap tmpPixMap;
    tmpPixMap.load(m_pressed2);
    this->setIcon(tmpPixMap);
    m_cur_state = PicBtnState2Pressed;
}

bool PicStateBtn::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        if (m_cur_state < PicBtnState2Normal) setHoverIcon();
        else setHover2Icon();
        break;
    case QEvent::Leave:
        if (m_cur_state < PicBtnState2Normal) setNormalIcon();
        else setNormal2Icon();
        break;
    case QEvent::MouseButtonPress:
        if (m_cur_state < PicBtnState2Normal) setPressIcon();
        else setPress2Icon();
        break;
    case QEvent::MouseButtonRelease:
        if (m_cur_state < PicBtnState2Normal) setHover2Icon();
        else setHoverIcon();
        break;
    default:
        break;
    }

    return QPushButton::event(event);
}

void PicStateBtn::slotStart()
{
    setNormal2Icon();
}

void PicStateBtn::slotStop()
{
    setNormalIcon();
}

