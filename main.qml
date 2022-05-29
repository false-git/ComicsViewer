import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Dialogs
import Qt.labs.settings
import Qt.labs.platform as Platform
import ComicsViewer

ApplicationWindow {
    id: appWindow
    title: qsTr("Comics Viewer") + ((comicModel.currentFilename === "") ? "" : " - " + comicModel.currentFilename + comicModel.currentPageName)
    width: 640
    height: 480
    visible: true

    Settings {
        property alias x: appWindow.x
        property alias y: appWindow.y
        property alias width: appWindow.width
        property alias height: appWindow.height
    }

    Platform.MenuBar {
        Platform.Menu {
            title: qsTr("&File")
            Platform.MenuItem {
                text: qsTr("&Open...")
                onTriggered: fileDialog.open()
            }
            Platform.Menu {
                id: recentFileMenu
                title: qsTr("Open &recent")
                Instantiator {
                    model: comicModel.recentFiles
                    onObjectAdded: (index, object) => recentFileMenu.insertItem(index, object)
                    onObjectRemoved: object => recentFileMenu.removeItem(object)
                    delegate: Platform.MenuItem {
                        text: modelData
                        onTriggered: comicModel.open("file://" + modelData)
                    }
                }
                Platform.MenuSeparator {

                }
                Platform.MenuItem {
                    text: qsTr("&Delete all")
                    onTriggered: comicModel.clearRecentFiles()
                }
            }
            Platform.MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }
    footer: ToolBar {
        Label {
            text: comicModel.errorMessage
        }
    }

    ComicModel {
        id: comicModel
        onCurrentPageChanged: function(currentPage) {
            pageSlider.pageNumber = currentPage
        }
    }

    Item {
        anchors.fill: parent

        PageItem {
            id: pageItem
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height - pageSlider.height
            pageImage: comicModel.currentImage
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (mouseX < width / 2) {
                        comicModel.nextPageset()
                    } else {
                        comicModel.previousPageset()
                    }
                }
            }
        }
        Slider {
            id: pageSlider
            stepSize: 1
            property int pageNumber: 1
            from: 0
            to: comicModel.maxPage < 2 ? 1 : comicModel.maxPage - 1
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: pageRect.left
            anchors.rightMargin: 10
            rotation: 180
            onValueChanged: {
                if (pageNumber != value + 1) {
                    pageNumber = value + 1
                    if (comicModel.currentPage !== pageNumber) {
                        comicModel.currentPage = pageNumber
                    }
                }
            }
            onPageNumberChanged: {
                if (value != pageNumber - 1) {
                    value = pageNumber - 1
                }
            }
        }
        Rectangle {
            id: pageRect
            border.color: "#000"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: pageText.contentWidth + 10
            height: pageSlider.height
            Text {
                id: pageText
                anchors.centerIn: parent
                text: pageSlider.pageNumber + " / " + comicModel.maxPage
            }
        }
    }

    FocusScope {
        id: focus
        focus: true
        Keys.onRightPressed: {
            comicModel.previousPage()
        }
        Keys.onLeftPressed: {
            comicModel.nextPage()
        }
        Keys.onUpPressed: {
            comicModel.previousPageset()
        }
        Keys.onDownPressed: {
            comicModel.nextPageset()
        }
    }

    FileDialog {
        id: fileDialog
        onAccepted: {
            comicModel.open(selectedFile)
            //comicModel.currentPage = 1
        }
        onVisibleChanged: {
            if (!visible) {
                focus.forceActiveFocus()
            }
        }
    }
}
