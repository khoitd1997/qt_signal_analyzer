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

    property var modeList: ["Live Mode", "Analyis Mode", "Device Config Mode"]
    property string signalSource: "Simulation"

    Label {
        id: actionStatusLabel
        visible: false
        rightPadding: 20
        anchors.right: modeLabel.left

        text: ""
        color: "#BEBCB5"
        font.pointSize: 12
    }

    Label {
        id: modeLabel
        rightPadding: 20
        anchors.right: deviceStatusLabel.left

        text: modeList[0]
        color: "#BEBCB5"
        font.pointSize: 12
    }

    Label {
        id: deviceStatusLabel
        rightPadding: 10
        anchors.right: parent.right

        text: "Signal Source: " + bottomBar.signalSource
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

    function changeActionStatus(newActionStatus) {
        actionStatusLabel.text = newActionStatus;
        actionStatusLabel.visible = true
        fadeTimer.running = true
    }

    function changeSignalSource(newSignalSource) {
        bottomBar.signalSource = newSignalSource;
    }

    function changeMode(newModeIndex) {
            modeLabel.text = modeList[newModeIndex];
    }
}
