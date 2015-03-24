import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0
import ComicsViewer 1.0

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

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open...")
                onTriggered: fileDialog.open()
            }
            Menu {
                id: recentFileMenu
                title: qsTr("Open &recent")
                Instantiator {
                    model: comicModel.recentFiles
                    onObjectAdded: recentFileMenu.insertItem(index, object)
                    onObjectRemoved: recentFileMenu.removeItem(object)
                    delegate: MenuItem {
                        text: modelData
                        onTriggered: comicModel.open("file://" + modelData)
                    }
                }
                MenuSeparator {

                }
                MenuItem {
                    text: qsTr("&Delete all")
                    onTriggered: comicModel.clearRecentFiles()
                }
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }
    statusBar: StatusBar {
        anchors.fill: parent
        Label {
            text: comicModel.errorMessage
        }
    }

    ComicModel {
        id: comicModel
        onCurrentPageChanged: {
            pageSlider.value = currentPage
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
            minimumValue: comicModel.maxPage === 0 ? 0 : 1
            maximumValue: comicModel.maxPage === 0 ? 1 : comicModel.maxPage
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: pageRect.left
            anchors.rightMargin: 10
            onValueChanged: {
                if (comicModel.currentPage !== value) {
                    comicModel.currentPage = value
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
                text: pageSlider.value + " / " + pageSlider.maximumValue
            }
        }
    }

    FocusScope {
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
            comicModel.open(fileUrl)
            //comicModel.currentPage = 1
        }
    }
}
