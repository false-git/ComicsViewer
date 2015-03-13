#include "pageitem.h"
#include <QPainter>
#include <QDebug>

PageItem::PageItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{

}

PageItem::~PageItem()
{

}

void PageItem::setPageImage(QImage arg)
{
    m_pageImage = arg;
    update();
}

void PageItem::paint(QPainter *painter)
{
    QRectF rect = boundingRect();
    QRect irect = m_pageImage.rect();
    if (irect.isValid()) {
        if (irect.width() / rect.width() > irect.height() / rect.height()) {
            qreal height = rect.height();
            rect.setHeight(rect.width() / irect.width() * irect.height());
            rect.moveTop((height - rect.height()) / 2);
        } else {
            qreal width = rect.width();
            rect.setWidth(rect.height() / irect.height() * irect.width());
            rect.moveLeft((width - rect.width()) / 2);
        }
    }
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->drawImage(rect, m_pageImage);
}

QImage PageItem::pageImage() const
{
    return m_pageImage;
}
