import QtQuick 2.12
import QtQuick.Controls 2.5


Rectangle {
    property string sliderText: "Default Slider Text"

    signal sliderMoved(real newRange)

    height: childrenRect.height
    color: "transparent"
    anchors.top: currentSignalButton.bottom

Label {
    leftPadding: 5
    id: timeScaleText
    font.bold: true
    font.pointSize: 13
    color: UIStyle.buttonTextColor
    text: sliderText
}

Slider {
id: timeScaleSlider
//            topPadding:
from: 1
value: 25
to: 50000
anchors.right: parent.right
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
        color: "#21be2b"
        radius: 2
    }
}

onMoved: {
    sliderMoved(value)
//    axisChanged(currentSignalButton.currentSelection, "x", value)
}
}
}
