import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.folderlistmodel 2.2
import QtQuick.Controls.Material 2.3

ApplicationWindow {
    id: root
    visible: true
    width: 1024
    height: 600
    title: qsTr("Images")
    Material.theme: Material.Dark

    property string imageLocation: "";
    property var currentFrame: undefined
    property var imageFilters : ["*.png", "*.jpg"];

    FileDialog {
        id: fileDialog
        title: "Выберите папку с изображениями"
        selectFolder: true
        folder: imageLocation
        onAccepted: {
           folderModel.folder = fileUrl + "/"
       }
    }
 FolderListModel {
                id: folderModel
                objectName: "folderModel"
                showDirs: false
                nameFilters: imageFilters
            }
 ListView {
     id: view
     anchors.margins: 10
     anchors.fill: parent
     model: folderModel
     spacing: 25
            delegate: Rectangle {
                    id: imageFrame
                    Material.theme: Material.Dark
                    Material.background: Material.Dark
                    width: 150
                    height: 150
                    Row {
                        anchors.left: parent.left
                Image {
                id: image
                width: 150
                height: 150
                source: folderModel.folder + fileName
                asynchronous: true
            }
                Column {
                Text {
                    color: "white"
                    text: "Name: " + imginfo.getFileName(folderModel.folder + fileName);

                }
                Text {
                    color: "white"
                    text: "Last edit: " + imginfo.getFileLastModified(folderModel.folder + fileName);
                }
                Text {
                    color: "white"
                    text: "Last open: " + imginfo.getFileLastRead(folderModel.folder + fileName);

                }
                }
                    }

            PinchArea {
                anchors.fill: parent
                pinch.target: imageFrame
                //pinch.minimumScale: 0.1
                //pinch.maximumScale: 10

                MouseArea {
                    id: dragArea
                    hoverEnabled: true
                    anchors.fill: parent
                    scrollGestureEnabled: false

                    onEntered: {
                        imageFrame.border.color = "black"
                        image.scale += 0.1
                    }
                    onExited: {
                        imageFrame.border.color = "white"
                        image.scale -= 0.1 ;
                    }
                }
                function setFrameColor() {
                    if (currentFrame)
                        currentFrame.border.color = "white";
                    currentFrame = imageFrame;
                    currentFrame.border.color = "black";
                }
            }
            }
 }

    Rectangle {
        id: verticalScroll
        anchors.right: parent.right
        anchors.margins: 2
        border.color: "black"
        border.width: 1
        width: 5
        radius: 2
        antialiasing: true
        //height: flick.height * (flick.height / flick.contentHeight) - (width - anchors.margins) * 2
        //y: flick.contentY * (flick.height / flick.contentHeight)
        NumberAnimation on opacity { id: vfade; to: 0; duration: 500 }
        onYChanged: { opacity = 1.0; scrollFadeTimer.restart() }
    }

//    Rectangle {
//        id: horizontalScroll
//        anchors.margins: 2
//        border.color: "black"
//        border.width: 1
//        height: 5
//        radius: 2
//        antialiasing: true
//        width: flick.width * (flick.width / flick.contentWidth) - (height - anchors.margins) * 2
//        x: flick.contentX * (flick.width / flick.contentWidth)
//        NumberAnimation on opacity { id: hfade; to: 0; duration: 500 }
//        onXChanged: { opacity = 1.0; scrollFadeTimer.restart() }
//    }

//    Timer {
//        id: scrollFadeTimer
//        interval: 1000
//        onTriggered: { hfade.start(); vfade.start() }
//    }

    Button {
        id: btn
        text: "Выберите папку..."
        anchors.top: parent.top
        anchors.right: parent.right
        Material.background: Material.Blue

        onClicked: {
            fileDialog.open()
        }
    }
}
