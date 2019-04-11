import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.2

Page {
    id: root

    header: ChatToolBar {
        ToolButton {
            Image {
                id: menuIco
                anchors.centerIn: parent
                source: "qrc:/images/images/menu.png"
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


    FileDialog {
        id: fileDialog
        title: "Выберите папку с изображениями"

        onAccepted: {
            cl.sendMessage("System", "Пользователь: " + user + " отправил(а) файл: " + basename(fileUrl.toString()))
            cl.sendFile(fileDialog.fileUrl, basename(fileUrl.toString()))
        }
    }

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: listView
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: pane.leftPadding + messageField.leftPadding
            displayMarginBeginning: 40
            displayMarginEnd: 40
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
                        source: "qrc:/images/images/sendFile.png"
                    }
                    onClicked: fileDialog.open()
                }

                RoundButton {
                    id: sendButton

                    Image {

                        id: sendMesIco
                        anchors.fill: parent
                        source: "qrc:/images/images/sendMessage.png"
                    }

                    onClicked: {
                        if(messageField.length > 0) {
                            cl.send_message(user, messageField.text);
                            messageField.clear()
                        }
                    }
                }
            }
        }
    }
}
