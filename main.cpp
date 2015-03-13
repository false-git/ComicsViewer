#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "comicmodel.h"
#include "pageitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 設定を保存するために、organization, applicationの情報を設定する
    app.setOrganizationName("wizard-limit");
    app.setOrganizationDomain("wizard-limit.net");
    app.setApplicationName("ComicsViewer");

    // QMLから参照するC++クラスを登録する
    qmlRegisterType<ComicModel>("ComicsViewer", 1, 0, "ComicModel");
    qmlRegisterType<PageItem>("ComicsViewer", 1, 0, "PageItem");

    // i18n
    QTranslator myappTranslator;
    myappTranslator.load(":/i18n/ComicsViewer_" + QLocale::system().name());
    app.installTranslator(&myappTranslator);

    // QMLを表示する
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
