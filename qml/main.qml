import QtQuick
import injector
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import presenters

Window {
    width: 1024
    height: 680
    visible: true
    title: qsTr("Hello World")

    ColumnLayout {

        QmlInjector {
            implicitWidth: view.width
            implicitHeight: view.height

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
                    color: model.mainColor
                }
            }
        }
        QmlInjector {
            implicitWidth: view.width
            implicitHeight: view.height
            sourceComponent: Column {
                property WorldControllerPresenter $presenter
                Text {
                    text: slider.value
                }
                Slider {
                    id: slider
                    property bool upd: true

                    Timer {
                        id: waitTimer
                        interval: 10000
                        repeat: false
                        running: false
                        onTriggered: {
                            parent.upd = true;
                        }
                    }

                    Timer {
                        interval: 1000
                        repeat: true
                        running: true



                        onTriggered: {
                            if (parent.upd) {
                                $presenter.setSunLevel(slider.value + 1);
                                if (slider.value > 10) {
                                    parent.upd = false;
                                }
                            } else {
                                if (slider.value > 0) {
                                    $presenter.setSunLevel(slider.value - 1);
                                }
                                if (slider.value === 0) {
                                    waitTimer.start();
                                }

                            }
                        }
                    }

                    from: 0
                    stepSize: 1
                    value: $presenter.sunLevel
                    to: 100
                    onValueChanged: {
                        console.log("helius: setSunLevel", value)
                        $presenter.setSunLevel(value);
                    }
                }
            }
        }
    }
}
