import QtQuick 2.12
import QtQuick.Controls 2.4

import "ControlPanel"
import "ScopeView"

Item {
    signal actionStatusChanged(string newActionStatus);
    ControlPanel {
        id: controlPanel
        anchors.topMargin: 2
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width / 3

        // graph chooser stuffs
        onAxisOffsetChanged: scopeView.changeAxisOffset(id, axisName, newOffset)
        onAxisRangeChanged: scopeView.changeAxisRange(id, axisName, newRange)
        onRefreshRateChanged: scopeView.changeRefreshRate(rate);
        onAntialiasingEnabled: scopeView.antialiasing = enabled;

        // cursor stuffs
        onCursorYChanged: scopeView.changeYCursor(serieIndex, cursorIndex, newPosition)
        onCursorXChanged: scopeView.changeXCursor(cursorIndex, newPosition)
        onCursorDisplayChanged: scopeView.cursorDisplayChanged(isOn)
    }

    ScopeView {
        id: scopeView
        anchors.top: parent.top
        anchors.topMargin: -7

        anchors.right: parent.right
        anchors.left: controlPanel.right
        height: parent.height * 0.8

        onNewCursorData: {
            controlPanel.xCursorDif = Math.round(xCursorDif * 10)/ 10;
            controlPanel.yCursorA = Math.round(yCursorA* 10)/ 10;
            controlPanel.yCursorB = Math.round(yCursorB* 10)/ 10;
            controlPanel.yCursorDif = Math.round(yCursorDif* 10)/ 10;
        }

        onOpenGLSupportedChanged: {
            if (!openGLSupported) {
                controlPanel.openGLButton.enabled = false
                controlPanel.openGLButton.currentSelection = 0
            }
        }
    }

//    MathControl {
//        id: mathControl
//        width: parent.width
//        height: childrenRect.height
//        anchors.left: scopeView.left
//        anchors.leftMargin: 10
//        anchors.top: scopeView.bottom
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//        anchors.topMargin: 1
//    }
}
