
#include "prelistitem.h"

PreListItem::PreListItem(const QIcon &icon, const QString &text, const int &index, QListWidget *listview, int type)
    : QListWidgetItem(icon, "", listview, type), m_path(text), m_index(index)
{

}

int PreListItem::getIndex()
{
    return m_index;
}

QString PreListItem::getPath()
{
    return m_path;
}
