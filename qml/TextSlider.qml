import QtQuick
import injector
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Row {
    property alias text: t.text
    property alias value: s.value
    property alias max: s.to
    property alias min: s.from
    property alias step: s.stepSize

    spacing: 10

    Text {
        id: t

    }
    Slider {
        id: s
    }
    Text {
        text: s.value
    }
}
