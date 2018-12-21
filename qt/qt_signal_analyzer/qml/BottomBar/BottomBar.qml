import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQml 2.12

Rectangle {
    id: bottomBar
    width: parent.width
    height: 25
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    color:"#2E2D2C"

    property string actionStatus: "Action Status Default"
    property string currMode: "Current Mode Default"
    property string devicePath: "/dev/default"

    onActionStatusChanged: {
        actionStatusLabel.visible = true
        fadeTimer.running = true
    }

    Label {
        id: actionStatusLabel
        visible: false
        rightPadding: 20
        anchors.right: modeLabel.left

        text: bottomBar.actionStatus
        color: "#BEBCB5"
        font.pointSize: 12
    }

    Label {
        id: modeLabel
        rightPadding: 20
        anchors.right: deviceStatusLabel.left

        text: bottomBar.currMode
        color: "#BEBCB5"
        font.pointSize: 12
    }

    Label {
        id: deviceStatusLabel
        rightPadding: 10
        anchors.right: parent.right

        text: "Device Connected at " + bottomBar.devicePath
        color: "#BEBCB5"
        font.pointSize: 12
    }

    Timer {
        id: fadeTimer
        interval: 3000
        repeat: false
        onTriggered: {
            actionStatusLabel.visible = false
        }
    }
}
