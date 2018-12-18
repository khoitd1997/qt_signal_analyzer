import QtQuick 2.12
import QtQuick.Controls 2.4
import "."

Rectangle {
    id: sectionHeader

    height: sectionText.height
    width: parent.width
    property bool isTicked: false
    signal isClicked(bool isOn)

    color: UIStyle.buttonBgUnhovered

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
        color: UIStyle.buttonTextColor
        height: Text.paintedHeight
        anchors.top: parent.top
        anchors.left: parent.left
        text: "Section Header"
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
        source: "qrc:down-arrow.png"
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
