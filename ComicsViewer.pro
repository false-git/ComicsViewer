TEMPLATE = app

QT += qml quick widgets
CONFIG += c++11

!defined(QUAZIP_DIR, var) {
    QUAZIP_DIR = $(HOME)/qt/quazip
}
!exists($$QUAZIP_DIR/include/quazip/quazip.h) {
    error("quazip not found")
}

QMAKE_INCDIR += $$QUAZIP_DIR/include
QMAKE_LIBDIR += $$QUAZIP_DIR/lib
QMAKE_RPATHDIR += $$QUAZIP_DIR/lib
QMAKE_LIBS += -lquazip1-qt6

macx: ICON = ComicsViewer.icns
win32: RC_FILE = ComicsViewer.rc

SOURCES += main.cpp \
    comicmodel.cpp \
    pageitem.cpp

RESOURCES += qml.qrc \
    i18n.qrc

CODECFORSRC = UTF-8
TRANSLATIONS = i18n/ComicsViewer_ja.ts
lupdate_only {
    SOURCES += *.qml
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    comicmodel.h \
    pageitem.h
