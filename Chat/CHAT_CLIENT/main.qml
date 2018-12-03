import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "CHAT APPLICATION"

    RowLayout {
        id: layout
        anchors.left: parent.left
        width: parent.width / 3
        height: parent.height
        spacing: 5
        Item {
            Layout.fillWidth: true
            Layout.minimumWidth: parent.width / 3
            Layout.preferredWidth: parent.width / 3
            Layout.maximumWidth: parent.width
            Layout.minimumHeight: parent.height
            Material.background: Material.Orange
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
            Layout.fillWidth: true
            Layout.preferredWidth: parent.width - (parent.width / 3)
            Layout.preferredHeight: parent.height
            Material.background: Material.Green

            Text {
                anchors.centerIn: parent
                text: parent.width + 'x' + parent.height
            }

            TextInput {
                id: textInput
                text: "Введите текст..."
                anchors.right: sendButton.left
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                height: 40
                width: (3 / 4) * parent.width

            }

            Button {
                id: sendButton
                visible: true
                text: "Отправить"
                Material.background: Material.Teal
                highlighted: true
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: parent.width / 4
                height: 40
            }
        }
    }
}
