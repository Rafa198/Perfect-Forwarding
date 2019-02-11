import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

ToolBar {
    id: toolBar
    Material.theme: Material.Dark
    Material.background: Material.LightBlue

    MouseArea {
        anchors.fill: parent

        onPressed: {
            previousX = mouseX
            previousY = mouseY
        }

        onMouseXChanged: {
            var dx = mouseX - previousX
            window.setX(window.x + dx)
        }

        onMouseYChanged: {
            var dy = mouseY - previousY
            window.setY(window.y + dy)
        }
    }
}
