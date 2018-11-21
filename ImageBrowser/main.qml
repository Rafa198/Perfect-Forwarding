import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.folderlistmodel 2.2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: root
    visible: true
    width: 1024
    height: 600
    title: "Images"
    Material.theme: Material.Dark

    property var currentFrame: undefined

    FileDialog {
        id: fileDialog
        property string imageLocation: "";
        title: "Выберите папку с изображениями"
        selectFolder: true
        folder: imageLocation        
        onAccepted: {
           folderModel.folder = fileUrl + "/"       
        }
    }

    FolderListModel {
        id: folderModel
        property var imageFilters : ["*.png", "*.jpg"];
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
        delegate: RowLayout {
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft
                    spacing: 10

                    Item {
                        id: imageFrame
                        Material.background: Material.Dark
                        width: 150
                        height: 150

                        Image {
                            id: image
                            width: 150
                            height: 150
                            source: folderModel.folder + fileName
                            asynchronous: true
                        }

                        PinchArea {
                            anchors.fill: parent
                            pinch.target: imageFrame

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
                                    image.scale -= 0.1;
                                }
                            }
                        }
                    }

                    Item {
                        Layout.minimumHeight: 150

                        ColumnLayout {
                            spacing: 5

                            Text {
                                color: "white"
                                text: " Name: " + imginfo.getFileName(folderModel.folder + fileName)
                            }

                            Text {
                                color: "white"
                                text: " Last edit: " + imginfo.getFileLastModified(folderModel.folder + fileName)
                            }


                            Text {
                                color: "white"
                                text: " Last open: " + imginfo.getFileLastRead(folderModel.folder + fileName)
                            }

                            Text {
                                color: "white"
                                text: " Size: " + fileSize + "Byte"
                            }
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
        NumberAnimation on opacity { id: vfade; to: 0; duration: 500 }
        onYChanged: { opacity = 1.0; scrollFadeTimer.restart() }
    }

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
