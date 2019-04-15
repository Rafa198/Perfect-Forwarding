import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.12
Page {
    id: root

    header: ChatToolBar {
        ToolButton {
            Image {
                id: menuIco
                anchors.centerIn: parent
                source: "image/menu.png"
            }

            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter

            onClicked: {
                menuTB.open()
            }
        }

        Label {
            id: pageTitle
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }

    Menu {
        id: menuTB
        height: parent.height
        width: parent.width / 3

        MenuItem {
            text: "Exit"
            onClicked: {
                cl.sendMessage(user, "left a chat")
                window.close()
            }
        }
    }

    function basename(str){
            return (str.slice(str.lastIndexOf("/") + 1))
    }

    ProgressBar {
        z: 4
        visible: false

        id: control
        anchors.centerIn: parent
        value: 0
        height: 20
        clip: true
        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 6
            border.color: "#999999"
            radius: 5
        }
        contentItem: Item {
            implicitWidth: 200
            implicitHeight: 4

            Rectangle {
                id: bar
                width: control.visualPosition * parent.width
                height: parent.height
                radius: 5
            }

            LinearGradient {
                anchors.fill: bar
                start: Qt.point(0, 0)
                end: Qt.point(bar.width, 0)
                source: bar
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#17a81a" }
                    GradientStop { id: grad; position: 0.5; color: Qt.lighter("#17a81a", 2) }
                    GradientStop { position: 1.0; color: "#17a81a" }
                }
                PropertyAnimation {
                    target: grad
                    property: "position"
                    from: 0.1
                    to: 0.9
                    duration: 1000
                    running: true
                    loops: Animation.Infinite
                }
            }
            LinearGradient {
                anchors.fill: bar
                start: Qt.point(0, 0)
                end: Qt.point(0, bar.height)
                source: bar
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.rgba(0,0,0,0) }
                    GradientStop { position: 0.5; color: Qt.rgba(1,1,1,0.3) }
                    GradientStop { position: 1.0; color: Qt.rgba(0,0,0,0.05) }
                }
            }
        }
        PropertyAnimation {
            target: control
            property: "value"
            from: 0
            to: 1
            duration: 5000
            running: true
            loops: Animation.Infinite
        }
    }


    Timer {
        id: timer
        interval: 3500
        repeat: false
        onTriggered: control.visible = false
    }
    FileDialog {
        id: fileDialog
        title: "Выберите папку с изображениями"

        onAccepted: {
            cl.sendMessage("System", "Пользователь: " + user + " отправил(а) файл: " + basename(fileUrl.toString()))
            cl.sendFile(fileDialog.fileUrl, basename(fileUrl.toString()))
            control.visible = true
            timer.start()
        }
    }

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: listView
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: pane.leftPadding + messageField.leftPadding
            verticalLayoutDirection: listView.BottomToTop
            spacing: 12
            model: chmod

            delegate: Column {
                anchors.left: parent.left
                spacing: 6

                Row {
                    id: messageRow
                    spacing: 6
                    anchors.left: parent.left

//                    Image {
//                        id: avatar
//                        source: "file"
//                    }

                    Rectangle {
                        width: Math.min(messageText.implicitWidth + 24, listView.width - /*avatar.width -*/ messageRow.spacing)
                        height: messageText.implicitHeight + 24
                        color: Material.color(Material.LightBlue)

                        Text {
                            id: uname
                            text: " " + userName
                            font.pixelSize: 15
                            color: "white"
                        }

                        Label {
                            id: messageText
                            text: "\n" + message
                            anchors.fill: parent
                            anchors.margins: 12
                            wrapMode: Label.Wrap                            
                        }
                    }
                }
            }
            ScrollBar.vertical: ScrollBar {}

            Timer {
                id: positionTimer
                interval: 200
                repeat: false
                onTriggered: listView.positionViewAtIndex(listView.count - 1, ListView.Visible)
            }

            onCountChanged: {
                positionTimer.start()
            }
        }

        Pane {            
            id: pane
            Layout.fillWidth: true

            RowLayout {                
                width: parent.width

                TextArea {
                    id: messageField
                    Layout.fillWidth: true
                    placeholderText: "Compose message"
                    wrapMode: TextArea.Wrap
                }

                RoundButton {
                    id: sendFile

                    Image {
                        id: sendFileIco
                        anchors.fill: parent
                        source: "image/sendFile.png"
                    }
                    onClicked: fileDialog.open()
                }

                RoundButton {
                    id: sendButton

                    Image {

                        id: sendMesIco
                        anchors.fill: parent
                        source: "image/sendMessage.png"
                    }

                    onClicked: {
                        if(messageField.length > 0) {
                            cl.sendMessage(user, messageField.text);
                            messageField.clear()
                        }
                    }
                }
            }
        }
    }
}
