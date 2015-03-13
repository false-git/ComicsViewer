#include "comicmodel.h"

#include <QImage>
#include <QUrl>
#include <QDebug>
#include <QFile>
#include <QList>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QSettings>

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

#define RECENT_NUM 20
#define RECENTFILE_KEY "recent.%1"
#define RECENTPAGE_KEY "recentPage.%1"

/*!
 * \brief ページデータを表すクラス
 */
class PageData {
public:
    int pageNumber; // ページ(1〜)
    QString fileName; // 画像ファイル名
    int nop; // 1ページ表示 or 2ページ表示
};

ComicModel::ComicModel(QObject *parent) : QAbstractListModel(parent), m_zip(0), m_currentPage(0)
{
    restoreRecentFile();
}

ComicModel::~ComicModel()
{
    close();
}

bool ComicModel::open(const QString &fileUrl)
{
    if (m_zip) {
        close();
    }
    QUrl url(fileUrl);
    m_zip = new QuaZip(url.toLocalFile());
    m_zip->setFileNameCodec("Shift-JIS");
    bool result = m_zip->open(QuaZip::mdUnzip);
    if (result) {
        QStringList fileNameList = m_zip->getFileNameList();
        fileNameList.sort();
        for (int i = 0; i < fileNameList.size(); i++) {
            if (fileNameList[i].endsWith(".jpg")) {
                PageData *d = new PageData;
                d->pageNumber = i + 1;
                d->fileName = fileNameList[i];
                d->nop = 0;
                m_list.append(d);
            }
        }
        setErrorMessage();
        addRecentFile(url.toLocalFile());
    } else {
        setErrorMessage("open file error.");
    }
    emit maxPageChanged(m_list.size());
    m_currentFilename = url.toLocalFile();
    emit currentFilenameChanged(m_currentFilename);
    QSettings settings;
    QString key = QString(RECENTPAGE_KEY).arg(m_currentFilename);
    int page = settings.value(key, 1).toInt();
    setCurrentPage(page);
    return result;
}

void ComicModel::close()
{
    if (m_zip) {
        m_zip->close();
        delete m_zip;
        // 読んでいたページの保存。削除がないので、recent_fileを消すときに一緒に消した方が良いかも
        QSettings settings;
        QString key = QString(RECENTPAGE_KEY).arg(m_currentFilename);
        settings.setValue(key, m_currentPage);
    }
    for (int i = 0; i < m_list.size(); i++) {
        delete m_list[i];
    }
    m_list.clear();
    emit maxPageChanged(m_list.size());
    m_currentPage = 0;
    emit currentPageChanged(m_currentPage);
    m_currentFilename = "";
    emit currentFilenameChanged(m_currentFilename);
}

void ComicModel::setCurrentPage(int arg)
{
    //qDebug() << "setCurrentPage" << m_currentPage << arg;
    if (m_currentPage == arg) {
        return;
    }
    if (arg < 1 || arg > m_list.size()) {
        return;
    }
    PageData *data = m_list[arg - 1];
    m_currentPage = arg;
    QImage right = loadImage(arg);
    QImage img = right;
    data->nop = 1;
    if (right.width() < right.height()) {
        if (arg + 1 <= m_list.size()) {
            QImage left = loadImage(arg + 1);
            if (left.width() < left.height()) {
                int height = left.height() > right.height() ? left.height() : right.height();
                int width = left.width() + right.width();
                img = QImage(width, height, QImage::Format_ARGB32);
                QPainter painter(&img);
                painter.drawImage(0, 0, left);
                painter.drawImage(left.width(), 0, right);
                data->nop = 2;
            }
        }
    }
    m_currentImage = img;
    emit currentPageChanged(m_currentPage);
    emit currentImageChanged(m_currentImage);
}

void ComicModel::nextPage()
{
    setCurrentPage(m_currentPage + 1);
}

void ComicModel::previousPage()
{
    setCurrentPage(m_currentPage - 1);
}

void ComicModel::nextPageset()
{
    if (m_currentPage && m_list[m_currentPage - 1]->nop) {
        setCurrentPage(m_currentPage + m_list[m_currentPage - 1]->nop);
    } else {
        setCurrentPage(m_currentPage + 1);
    }
}

void ComicModel::previousPageset()
{
    if (m_currentPage <= 1) {
        return;
    }
    if (m_currentPage == 2) {
        setCurrentPage(1);
        return;
    }
    // 既に読んだページ以外は正しくないが、画像を読み込むコストをかけるよりは
    // これで良いことにする。
    if (m_list[m_currentPage - 3]->nop == 2) {
        setCurrentPage(m_currentPage - 2);
    } else {
        setCurrentPage(m_currentPage - 1);
    }
}

void ComicModel::clearRecentFiles()
{
    m_recentFiles.clear();
    QSettings settings;
    for (int i = 0; i < RECENT_NUM; i++) {
        QString key = QString(RECENTFILE_KEY).arg(i);
        settings.remove(key);
    }
    emit recentFilesChanged(m_recentFiles);
}

QImage ComicModel::loadImage(int arg)
{
    QImage img;
    --arg;
    //qDebug() << m_list[arg]->fileName;
    if (m_zip->setCurrentFile(m_list[arg]->fileName)) {
        QuaZipFile file(m_zip);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            file.close();
            if (img.loadFromData(data)) {
                //qDebug() << "load success" << data.size();
                setErrorMessage();
            } else {
                //qDebug() << "loadFromData error.";
                setErrorMessage("error: loadFromData()");
            }
        } else {
            //qDebug() << "file open error.";
            setErrorMessage("error: file.open()");
        }
    } else {
        //qDebug() << "setCurrentFile failed." << m_zip->getZipError();
        setErrorMessage("error: setCurrentFile()");
    }
    return img;
}

void ComicModel::setErrorMessage(const QString &message)
{
    m_errorMessage = message;
    emit errorMessageChanged(message);
}

void ComicModel::addRecentFile(QString filename)
{
    if (m_recentFiles.contains(filename)) {
        m_recentFiles.removeOne(filename);
    } else {
        while (m_recentFiles.size() >= RECENT_NUM) {
            m_recentFiles.removeLast();
        }
    }
    m_recentFiles.insert(0, filename);
    emit recentFilesChanged(m_recentFiles);
    QSettings settings;
    for (int i = 0; i < m_recentFiles.size(); i++) {
        QString key = QString(RECENTFILE_KEY).arg(i);
        settings.setValue(key, m_recentFiles[i]);
    }
}

void ComicModel::restoreRecentFile()
{
    QSettings settings;
    m_recentFiles.clear();
    for (int i = 0; i < RECENT_NUM; i++) {
        QString key = QString(RECENTFILE_KEY).arg(i);
        QString filename = settings.value(key, "").toString();
        if (filename != "") {
            m_recentFiles.append(filename);
        }
    }
}

int ComicModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.size();
}

QVariant ComicModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_list.size()) {
        return QVariant();
    }
    PageData *d = m_list[row];
    switch (role) {
    case PageNumberRole:
        return d->pageNumber;
    case FileNameRole:
        return d->fileName;
    case ImageRole:
        return QVariant();
    }
    return QVariant();
}

QHash<int, QByteArray> ComicModel::roleNames() const
{
    static QHash<int, QByteArray> roleNames;
    if (roleNames.size() == 0) {
        roleNames[PageNumberRole] = "pageNumber";
        roleNames[FileNameRole] = "fileName";
        roleNames[ImageRole] = "image";
    }
    return roleNames;
}

int ComicModel::currentPage() const
{
    return m_currentPage;
}

QImage ComicModel::currentImage() const
{
    return m_currentImage;
}

int ComicModel::maxPage() const
{
    return m_list.size();
}

QString ComicModel::errorMessage() const
{
    return m_errorMessage;
}

QStringList ComicModel::recentFiles() const
{
    return m_recentFiles;
}

QString ComicModel::currentFilename() const
{
    return m_currentFilename;
}

