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
                spacing: 10
                property WorldControllerPresenter $presenter
                Row {
                    spacing: 10
                    TextSlider {
                        id: slider
                        property bool upd: true
                        text: "Sun energy"
                        min: 0
                        step: 1
                        value: $presenter.sunLevel
                        max: 100

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
                            id: tickTimer
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

                        onValueChanged: {
                            console.log("helius: setSunLevel", value)
                            $presenter.setSunLevel(value);
                        }
                    }
                    Switch {
                        id: pauseSwitch
                        text: "pause"
                        onCheckedChanged: {
                            if (checked) {
                                tickTimer.stop();
                            } else {
                                tickTimer.start();
                            }
                        }
                    }
                }
                TextSlider {
                    text: "Move energy"
                    min: 0
                    max: 500
                    step: 1
                    value: $presenter.moveEnergy
                    onValueChanged: {
                        $presenter.moveEnergy = value;
                    }
                }
                TextSlider {
                    text: "Mutation rate"
                    min: 0
                    max: 100
                    step: 10
                    value: $presenter.mutationRate
                    onValueChanged: {
                        $presenter.mutationRate = value;
                    }
                }
                TextSlider {
                    text: "Divide energy"
                    min: 0
                    max: 2000
                    step: 1
                    value: $presenter.divideEnergy
                    onValueChanged: {
                        $presenter.divideEnergy = value;
                    }
                }
                TextSlider {
                    text: "Max energy"
                    min: 0
                    max: 2000
                    step: 100
                    value: $presenter.maxEnergy
                    onValueChanged: {
                        $presenter.maxEnergy = value;
                    }
                }
                TextSlider {
                    text: "World speed: " + Math.pow(10, value)
                    min: 0
                    max: 2
                    step: 1
                    value: 0
                    onValueChanged: {
                        $presenter.worldSpeed = Math.pow(10, value);
                    }
                }
                TextSlider {
                    text: "Show mode"
                    min: 0
                    max: 2
                    step: 1
                    value: 0
                    onValueChanged: {
                        $presenter.showMode = value;
                    }
                }
            }
        }
    }
}
