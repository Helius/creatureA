import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    GridView {
        width: 246*4
        height: 128*4
        model: 128*256
        cellWidth: 4
        cellHeight: 4
        delegate: Rectangle {
            width: 3
            height: 3
            border.width: 0.5
            border.color: "red"
            color: "transparent"
        }
    }
}
