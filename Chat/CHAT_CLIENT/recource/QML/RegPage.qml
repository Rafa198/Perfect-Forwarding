import QtQuick 2.4
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3

Page {
    id: root

    header: ChatToolBar {
        Label {
            text: "Авторизация"
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }

    ColumnLayout {
        parent: root
        anchors.fill: parent

        Image {
            id: regIco
            Layout.alignment: Qt.AlignCenter
            source: "image/chat.png"
        }

        TextField {
            id: login
            Layout.preferredWidth: parent.width / 2
            Layout.alignment: Qt.AlignCenter
            placeholderText: "Введите имя"
        }

        Button {
            id: enterButton
            Layout.preferredWidth: parent.width / 2
            Layout.alignment: Qt.AlignCenter
            text: "Войти"
            enabled: login.text.length > 0

            onClicked: {
                if(login.text.length > 0) {
                    user = login.text
                    root.StackView.view.push("qrc:/ChatPage.qml")
                    cl.send_message(user,"entered a chat")
                }
            }
        }
    }
}
