import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
//import client 1.0

ApplicationWindow {
    id: main
    visible: true
    //flags: Qt.FramelessWindowHint
    width: 640
    height: 480
    title: "CHAT APPLICATION"

    RowLayout {
        id: layout
        anchors.left: parent.left
        width: parent.width / 4
        height: parent.height
        spacing: 5

        Item {
            Layout.fillWidth: true
            Layout.minimumWidth: parent.width / 3
            Layout.preferredWidth: parent.width / 3
            Layout.maximumWidth: parent.width
            Layout.minimumHeight: parent.height
            Material.background: Material.Grey

            Text {
                anchors.centerIn: parent
                text: parent.width + 'x' + parent.height
            }
        }
    }

    ColumnLayout {
        id: columnLayout
        anchors.left: layout.right
        height: parent.height
        width: parent.width - layout.width
        spacing: 5

        Item {
            id: mainItem
            Layout.fillWidth: true
            Layout.preferredWidth: parent.width - (parent.width / 3)
            Layout.preferredHeight: parent.height
            Material.background: Material.Grey

//            Text {
//                anchors.centerIn: parent
//                text: parent.width + 'x' + parent.height
//            }

            Item {
                id: messageRec
                anchors.right: parent.right
                anchors.bottom: pane.top
                anchors.left: parent.left
                anchors.top: parent.top
                //color: "red"

                ListModel {
                    id: textModel
                    ListElement {
                        msg: "cl.getMsg(1)"
                    }
                }

                ColumnLayout {
                    anchors.fill: parent

                ListView {
                    id: viewM
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: pane.leftPadding + messageField.leftPadding
                    displayMarginBeginning: 40
                    displayMarginEnd: 40
                    verticalLayoutDirection: ListView.BottomToTop
                    spacing: 12
                    model: textModel
                    delegate: RowLayout {
                     Layout.maximumWidth: parent.height / 2
                     spacing: 5

                     Item {
                         id: msgItem
                         width: 200
                         height: 50

                         Text {
                             color: "white"
                             text: "message " + msg
                         }
                     }
                    }
                    ScrollBar.vertical: ScrollBar {}
                }
                }
            }

            Pane {
                id: pane
                anchors.bottom: mainItem.bottom
                width: mainItem.width
                Layout.fillWidth: true
                background: Material.color(Material.Grey)

                RowLayout {
                    width: mainItem.width

                    TextArea {
                        id: messageField                        
                        Layout.fillWidth: true
                        placeholderText: "Compose message"
                        wrapMode: TextArea.Wrap
                    }

                    Button {
                        id: sendButton
                        visible: true
                        text: "Отправить"
                        Material.background: Material.BlueGrey
                        highlighted: true
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        width: parent.width / 4
                        height: 40
                        enabled: messageField.length > 0

                        onClicked: {
                            if(messageField.length != 0) {
                             cl.send1("USER0000", messageField.text);
                            //cl.client_run(messageField.text, "USER")
                            messageField.clear()
                            }
                            else {
                                console.log("empty text input!!!")
                            }
                        }
                    }
                }
            }

//            TextInput {
//                id: txtInput
//                text: "Введите текст..."
//                anchors.right: sendButton.left
//                anchors.left: parent.left
//                anchors.bottom: parent.bottom
//                selectByMouse: true
//                selectionColor: "green"
//                font.pixelSize: 14
//                color: "white"
//                height: 40
//                width: (3 / 4) * parent.width
//                //horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter
////                Keys.onPressed: {
////                    if(event.key === 16777220) {
////                        if(txtInput.length != 0) {
////                        cl.client_run(txtInput.text)
////                        txtInput.clear()
////                        }
////                        else {
////                            console.log("empty textInput!!!")
////                        }
////                    }
//                }

////                MouseArea{
////                    id: marea
////                    visible: false
////                    anchors.fill: txtInput

////                    onClicked: {
////                        txtInput.clear()
////                    }

////                }
//           }


        }
    }
}
