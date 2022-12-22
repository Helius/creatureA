import QtQuick
import injector
import presenters

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    QmlInjector {
        anchors.fill: parent

        sourceComponent: GridView {
            property WorldMapModel $worldModel
            model: $worldModel
            width: 246*4
            height: 128*4
            cellWidth: 4
            cellHeight: 4

            delegate: Rectangle {
                width: 3
                height: 3
                border.width: 1
                border.color: "gray"// model.borderColor
                color: model.mainColor
            }
        }
    }
}
