import QtQuick 2.12
import QtQuick.Controls 2.4
import "."

Rectangle {
    id: sectionHeader

    height: sectionText.height
    width: parent.width
    property bool isTicked: false
    property string displayText: "Default Collapsible Header"
    signal isClicked(bool isOn)
    radius: 4
    // color: "#3A3A3A"
    color: "#363636"

    border.color: isTicked ? "#56825D" : "transparent"

    MouseArea {
        anchors.fill: parent;
        onClicked: {
            sectionHeader.isTicked ? sectionIconUp.running = true : sectionIconDown.running = true
            sectionHeader.isClicked(isTicked)
            sectionHeader.isTicked = sectionHeader.isTicked ? false : true
        }
    }

    Label {
        id: sectionText
        leftPadding: 10
        font.pointSize: 15
        color: "#86CB92"
        height: Text.paintedHeight
        anchors.top: parent.top
        anchors.left: parent.left
        text: sectionHeader.displayText
        background: Rectangle {
            color: "transparent"

            anchors.fill: parent
        }
    }


    Image {
        anchors.rightMargin: 2
        height: parent.height
        anchors.right: parent.right
        anchors.top: parent.top
        fillMode: Image.PreserveAspectFit
        rotation: 90
        id: sectionIcon
        source: "qrc:///qml/resources/down-arrow.png"
    }

    RotationAnimator {
        id: sectionIconDown
        target: sectionIcon;
                from: 90;
                to: 0;
                duration: 100
    }

    RotationAnimator {
        id: sectionIconUp
        target: sectionIcon;
                from: 0;
                to: 90;
                duration: 100
    }

}
