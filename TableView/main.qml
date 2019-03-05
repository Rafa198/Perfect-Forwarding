import QtQuick 2.11
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Main"

    FileDialog {
        id: fileDialog
        title: "Выберите файл"

        onAccepted: {
            tModel.add(fileDialog.fileUrl)
        }
    }

    Button {
        id: fileDialogButton
        text: "File..."
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: 40
        height: 40
        onClicked: {
            fileDialog.open()
        }
    }

    TableView {
        id: tableView

        frameVisible: false
        sortIndicatorVisible: true

        width: parent.width
        Layout.minimumWidth: 400
        Layout.minimumHeight: 240
        Layout.preferredWidth: 600
        Layout.preferredHeight: 400

        model: tModel

        TableViewColumn {
            id: idColumn
            title: "id"
            role: "userID"
            movable: false
            resizable: false
        }

        TableViewColumn {
            id: uName
            title: "Name"
            role: "userName"
            movable: false
            resizable: false

        }

        TableViewColumn {
            id: uPhoneNumber
            title: "Phone Number"
            role: "userPhoneNumber"
            movable: false
            resizable: false
        }
    }
}



