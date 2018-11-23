import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.folderlistmodel 2.2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import testmodel 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 1024
    height: 600
    title: "Images"
    Material.theme: Material.Dark

    property var currentFrame: undefined
    property string imageLocation: "";

    FileDialog {
        id: fileDialog
        title: "Выберите папку с изображениями"
        selectFolder: true
        folder: imageLocation
        onAccepted: {
            imageLocation = fileDialog.folder + "/"
            tmod.add(imageLocation);
        }
    }

    ListView {
        id: view
        anchors.margins: 10
        anchors.fill: parent
        model: tmod
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
                            source: "file:///" + fileUrl
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
                                    image.scale += 0.1
                                }

                                onExited: {
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
                                text: " Name: " + imginfo.getFileName(fileUrl)
                            }

                            Text {
                                color: "white"
                                text: " Last edit: " + lastEdit
                            }


                            Text {
                                color: "white"
                                text: " Last open: " + lastRead
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
