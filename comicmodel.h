#ifndef COMICMODEL_H
#define COMICMODEL_H

#include <memory>
#include <QAbstractListModel>
#include <QList>
#include <QImage>
#include <QBuffer>

class PageData;
class QuaZip;

/*!
 * \brief jpgファイルが入ったzipファイルを保持するモデル
 *
 * QAbstractListModelにしてあるが、現時点では特にQML側でListModelとしては扱っていない
 */
class ComicModel : public QAbstractListModel
{
    Q_OBJECT
    /*!
     * \property ComicModel::currentPage
     * \brief 現在のページ
     */
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    /*!
     * \property ComicModel::currentPageName
     * \brief 現在のページのファイル名
     */
    Q_PROPERTY(QString currentPageName READ currentPageName NOTIFY currentPageNameChanged)
    /*!
     * \property ComicModel::currentImage
     * \brief 現在のイメージ
     */
    Q_PROPERTY(QImage currentImage READ currentImage NOTIFY currentImageChanged)
    /*!
     * \property ComicModel::maxPage
     * \brief 最終ページ
     */
    Q_PROPERTY(int maxPage READ maxPage NOTIFY maxPageChanged)
    /*!
     * \property ComicModel::errorMessage
     * \brief エラーメッセージ
     */
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    /*!
     * \property ComicModel::recentFiles
     * \brief 最近開いたファイルリスト
     */
    Q_PROPERTY(QStringList recentFiles READ recentFiles NOTIFY recentFilesChanged)
    /*!
     * \property ComicModel::currentFilename
     * \brief 現在のファイル名
     */
    Q_PROPERTY(QString currentFilename READ currentFilename NOTIFY currentFilenameChanged)
public:
    /*!
     * \brief コンストラクタ
     * \param parent 親オブジェクト
     */
    explicit ComicModel(QObject *parent = 0);
    /*!
     * \brief デストラクタ
     */
    ~ComicModel();
    /*!
     * \brief The Role enum
     */
    enum Role {
        PageNumberRole = Qt::UserRole + 1,
        FileNameRole,
        ImageRole
    };

signals:
    /*!
     * \brief 現在のページが変更されたときにemitされるシグナル
     * \param currentPage 現在のページ
     */
    void currentPageChanged(int currentPage);
    /*!
     * \brief 現在のページが変更されたときにemitされるシグナル
     * \param currentPageName 現在のページのファイル名
     */
    void currentPageNameChanged(QString currentPageName);
    /*!
     * \brief 現在のページのイメージが変更されたときにemitされるシグナル
     * \param currentImage 現在のページのイメージ
     */
    void currentImageChanged(QImage currentImage);
    /*!
     * \brief 最終ページが変更されたときにemitされるシグナル
     * \param maxPage 最終ページ
     */
    void maxPageChanged(int maxPage);
    /*!
     * \brief エラーメッセージが変更されたときにemitされるシグナル
     * \param errorMessage エラーメッセージ
     */
    void errorMessageChanged(QString errorMessage);
    /*!
     * \brief 最近開いたファイルリストが更新されたときにemitされるシグナル
     * \param recentFiles 最近開いたファイルリスト
     */
    void recentFilesChanged(QStringList recentFiles);
    /*!
     * \brief 現在のファイルが変更されたときにemitされるシグナル
     * \param currentFilename 現在のファイル名
     */
    void currentFilenameChanged(QString currentFilename);

public slots:
    /*!
     * \brief zipファイルを開く
     * \param fileUrl ファイルURL
     * \return 成功したときtrue
     */
    bool open(const QString &fileUrl);
    /*!
     * \brief ファイルを閉じる
     */
    void close();
    /*!
     * \brief 現在のページを設定する
     * \param currentPage 現在のページ
     */
    void setCurrentPage(int currentPage);
    /*!
     * \brief 次のページへ進む
     */
    void nextPage();
    /*!
     * \brief 前のページへ戻る
     */
    void previousPage();
    /*!
     * \brief 次のページへ進む
     *
     * nextPage()との違いは、現在のページが見開きページの場合は次のページへ、
     * 見開きページでない場合は次の次のページへ進む
     */
    void nextPageset();
    /*!
     * \brief 前のページへ戻る
     *
     * previousPage()との違いは、前のページが見開きページの時は前のページへ、
     * 前のページと前の前のページが共に見開きページでないときは前の前のページへ進む
     */
    void previousPageset();
    /*!
     * \brief 最近開いたファイルリストをクリアする
     */
    void clearRecentFiles();

private:
    /*!
     * \brief ページデータのリスト
     */
    QList<std::shared_ptr<PageData>> m_list;
    /*!
     * \brief zipバッファのリスト
     */
     QList<std::shared_ptr<QBuffer>> m_buffers;
    /*!
     * \brief 現在のページ(右)
     */
    int m_currentPage;
    /*!
     * \brief 現在のイメージ
     */
    QImage m_currentImage;
    /*!
     * \brief エラーメッセージ
     */
    QString m_errorMessage;
    /*!
     * \brief 最近開いたファイルリスト
     */
    QStringList m_recentFiles;
    /*!
     * \brief 現在のファイル名
     */
    QString m_currentFilename;
    /*!
     * \brief 画像を読みこむ
     * \param page 読み込むページ
     * \return ページ画像
     */
    QImage loadImage(int page);
    /*!
     * \brief エラーメッセージを設定する
     * \param message エラーメッセージ
     */
    void setErrorMessage(const QString &message = "");
    /*!
     * \brief 最近開いたファイルを追加する
     * \param filename ファイル名
     */
    void addRecentFile(QString filename);
    /*!
     * \brief 最近開いたファイルを設定から読み込む
     */
    void restoreRecentFile();
    /*!
     * \brief zipファイルを調べる
     * \param path zipファイルのパス
     * \param zip zipファイル
     * \param pageCount このzipファイルに含まれるページの数(サブzipは含まない)
     * \return 一つでもjpgがあればtrue
     */
    bool parseZip(const QString &path, std::shared_ptr<QuaZip> zip, int &pageCount);

public:
    /*!
     * \brief QAbstractListModel::rowCount() のオーバーライド
     * \param parent 親インデックス
     * \return ページ数
     */
    int rowCount(const QModelIndex &parent) const;
    /*!
     * \brief QAbstractListModel::data() のオーバーライド
     * \param index インデックス
     * \param role 取得するロール
     * \return データ
     */
    QVariant data(const QModelIndex &index, int role) const;
    /*!
     * \brief QAbstractListModel::roleNames() のオーバーライド
     * \return ロールのhash
     */
    QHash<int, QByteArray> roleNames() const;
    /**
     * @brief 現在のページを返す
     * @return 現在のページ
     */
    int currentPage() const;
    /**
     * @brief 現在のページのファイル名を返す
     * @return 現在のページのファイル名
     */
    QString currentPageName() const;
    /*!
     * \brief 現在の画像を返す
     * \return 現在の画像
     */
    QImage currentImage() const;
    /**
     * @brief 最終ページを返す
     * @return 最終ページ
     */
    int maxPage() const;
    /*!
     * \brief エラーメッセージを返す
     * \return エラーメッセージ
     */
    QString errorMessage() const;
    /*!
     * \brief 最近開いたファイルリストを返す
     * \return 最近開いたファイル
     */
    QStringList recentFiles() const;
    /*!
     * \brief 現在のファイル名を返す
     * \return 現在のファイル名
     */
    QString currentFilename() const;
};

#endif // COMICMODEL_H
