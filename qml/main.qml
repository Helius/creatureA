import QtQuick
import injector
import presenters

Window {
    width: 1024
    height: 680
    visible: true
    title: qsTr("Hello World")

    QmlInjector {
        anchors.fill: parent

        sourceComponent: GridView {
            property WorldMapModel $worldModel
            model: $worldModel
            width: 450*4
            height: 200*4
            cellWidth: 4
            cellHeight: 4

            delegate: Rectangle {
                width: 3
                height: 3
//                border.width: 1
//                border.color: model.borderColor
                color: model.mainColor
            }
        }
    }
}
