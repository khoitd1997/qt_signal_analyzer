import QtQuick 2.12
import QtQuick.Controls 2.5

Rectangle {
    id: control
    property string sliderText: "Default Slider Text"
    property real sliderFrom: 0
    property real sliderTo: 100
    property real sliderDefaultVal: 25

    property color sliderEnableColor: "#BB63BF"
    property color sliderTextColor: "#7E7F7B"

    signal sliderMoved(real newRange)

    height: childrenRect.height
    color: "transparent"

    Label {
        leftPadding: 5
        id: timeScaleText
        font.bold: true
        font.pointSize: 13
        color: control.sliderTextColor
        text: sliderText
    }

    Slider {
        anchors.right: parent.right

        id: timeScaleSlider
        from: sliderFrom
        to: sliderTo
        value: sliderDefaultVal

        background: Rectangle {
            x: timeScaleSlider.leftPadding
            y: timeScaleSlider.topPadding + timeScaleSlider.availableHeight / 2 - height / 2
            implicitWidth: 200
            implicitHeight: 4
            width: timeScaleSlider.availableWidth
            height: implicitHeight
            radius: 2
            color: "#bdbebf"

            Rectangle {
                width: timeScaleSlider.visualPosition * parent.width
                height: parent.height
                color: control.sliderEnableColor
                radius: 2
            }
        }

        onMoved: {
        sliderMoved(value)
        }
    }
}
