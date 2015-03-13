#ifndef PAGEITEM_H
#define PAGEITEM_H

#include <QQuickPaintedItem>
#include <QImage>

/*!
 * \brief QML表示用のページ描画Item
 */
class PageItem : public QQuickPaintedItem
{
    Q_OBJECT
    /*!
     * \property PageItem::pageImage
     * \brief 表示するImage
     */
    Q_PROPERTY(QImage pageImage READ pageImage WRITE setPageImage)
    /*!
     * \brief 表示するImage
     */
    QImage m_pageImage;

public:
    /*!
     * \brief コンストラクタ
     * \param parent 親Item
     */
    explicit PageItem(QQuickItem *parent = 0);
    /*!
     * \brief デストラクタ
     */
    ~PageItem();

signals:

public slots:
    /*!
     * \brief 表示するImageを設定する
     * \param pageImage 表示するImage
     */
    void setPageImage(QImage pageImage);

public:
    /*!
     * \brief QQuickPaintedItem::paint()のオーバーライド
     * \param painter QPainter*
     * 画像を描画する
     */
    void paint(QPainter *painter);
    /*!
     * \brief 表示するImageを取得する
     * \return 表示するImage
     */
    QImage pageImage() const;
};

#endif // PAGEITEM_H
