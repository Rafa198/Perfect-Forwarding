import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: main
    visible: true
    width: 640
    height: 480
    title: "CHAT APPLICATION"

    property string user: ""

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

    Rectangle {
        id: register
        width: 400
        height: 200
        anchors.centerIn: parent
        color: Material.color(Material.LightBlue)

        TextInput {
            id: userNameInput
            width: register.width / 2
            height: 25
            anchors.centerIn: parent
            text: "Enter NickName"
            font.pointSize: 10

            Label {
                id: textInputLabel
                width: userNameInput.width
                anchors.bottom: userNameInput.top
                text: "Name:"
            }
        }

        Button {
            id: regUser
            width: register.width
            height: 50
            anchors.bottom: register.bottom
            anchors.centerIn: register.bottom
            text: "Enter"

            onClicked: {
                if(userNameInput.text.length > 0 && userNameInput.text !== "") {
                    user = userNameInput.text
                    register.visible = false;
                    layout.visible = true;
                    columnLayout.visible = true;
                }
                else {
                    userNameInput.text = "Error! Enter a name!!!"
                }
            }
        }
    }

    RowLayout {
        id: layout
        width: parent.width / 4
        height: parent.height
        anchors.left: parent.left
        visible: false
        spacing: 5

        Item {
            Layout.fillWidth: true            
            Layout.preferredWidth: parent.width / 3
            Layout.maximumWidth: parent.width
            Layout.minimumHeight: parent.height
            Layout.minimumWidth: parent.width / 3

            ColumnLayout {
                width: layout.width

                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    text: "MENU"
                }

                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    text: "SETTINGS"
                }
            }
        }
    }

    ColumnLayout {
        id: columnLayout
        visible: false
        height: parent.height
        width: parent.width - layout.width
        anchors.left: layout.right        
        spacing: 5

        Item {
            id: mainItem
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width - (parent.width / 3)
            Layout.preferredHeight: parent.height - sendButton.height
            Material.background: Material.Grey

            Item {
                id: messageRec
                anchors.right: parent.right
                anchors.bottom: pane.top
                anchors.left: parent.left
                anchors.top: parent.top

                ColumnLayout {
                    anchors.fill: parent

                ListView {                   
                    id: viewM
                    Layout.fillWidth: true
                    Layout.preferredHeight: messageRec.height - pane.height - 25
                    Layout.margins: pane.leftPadding + messageField.leftPadding
                    verticalLayoutDirection: ListView.TopToBottom                    
                    displayMarginBeginning: 40
                    displayMarginEnd: 40                    
                    model: chmod
                    spacing: 6

                    delegate: RowLayout {                     
                        Layout.maximumWidth: parent.height / 2
                        spacing: 2

                     Rectangle {
                         id: msgItem
                         width: Math.min(text1.implicitWidth + 24, viewM.width - viewM.spacing)
                         height: text1.implicitHeight + 24
                         color: Material.color(Material.LightBlue)                        

                         Label {
                             id: text1
                             anchors.fill: parent
                             anchors.margins: 12
                             text: userName + "\n" + message
                             color: "white"
                             wrapMode: Label.Wrap
                         }                     
                     }
                    }

                    Flickable {                       
                        Layout.fillWidth: viewM.width
                        Layout.preferredHeight: viewM.height - 25

                        ScrollBar.vertical: ScrollBar {
                            id: scroll
                            parent: messageRec
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                            active: true                            

                            onHeightChanged: {
                                scroll.increase()
                            }
                        }
                    }
                }
                }
            }

            Pane {                
                id: pane
                width: mainItem.width
                anchors.bottom: mainItem.bottom                
                Layout.fillWidth: true
                background: Material.color(Material.Black)
                Material.foreground: Material.Black
                Material.elevation: 6
                Material.theme: Material.Dark

                RowLayout {
                    width: mainItem.width

                    Button {                       
                        id: sendFileButton
                        height: 45
                        width: parent / 4
                        Layout.alignment: Qt.AlignLeft | Qt.AlignBottom

                        Image {
                            height: parent
                            width: parent
                            source: "file:/QtProjects/chatproject/CHAT_CLIENT/res/upFile.png"
                        }

                        onClicked: {
                            fileDialog.open()
                        }
                    }

                    TextArea {
                        id: messageField                        
                        Layout.fillWidth: true
                        placeholderText: "Compose message"
                        wrapMode: TextArea.Wrap
                    }

                    Button {
                        id: sendButton
                        visible: true
                        width: parent.width / 4
                        height: 40
                        Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                        text: "Отправить"
                        Material.background: Material.Blue
                        highlighted: true
                        enabled: messageField.length > 0

                        onClicked: {
                            if(messageField.length > 0) {                             
                                cl.sendMessage(user, messageField.text);
                                messageField.clear()
                                scroll.increase()
                            }
                            else {
                                console.log("empty text input!!!")
                            }
                        }
                    }
                }
            }
        }
    }
}
