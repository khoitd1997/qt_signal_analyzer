import QtQuick 2.12
import QtQuick.Controls 2.4

import "ControlPanel"
import "ScopeView"

Item {
    ControlPanel {
        id: controlPanel
        anchors.topMargin: 2
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width / 3

        onAxisOffsetChanged: scopeView.changeAxisOffset(id, axisName, newOffset)
        onAxisRangeChanged: scopeView.changeAxisRange(id, axisName, newRange)
        onRefreshRateChanged: scopeView.changeRefreshRate(rate);
        onAntialiasingEnabled: scopeView.antialiasing = enabled;
        onSeriesNameChanged: scopeView.changeSeriesName(id, newName);
        onSeriesDisplayChanged: scopeView.changeSeriesDisplay(id, isOn);

        // cursor stuffs
        onYCursorChanged: scopeView.changeYCursor(serieIndex, cursorIndex, newPosition)
        onXCursorChanged: scopeView.changeXCursor(cursorIndex, newPosition)
        onCursorSwitch: scopeView.cursorSwitch(isOn)
    }

    ScopeView {
        id: scopeView
        anchors.top: parent.top
        anchors.topMargin: -7

        anchors.right: parent.right
        anchors.left: controlPanel.right
        height: parent.height * 0.8

        onOpenGLSupportedChanged: {
            if (!openGLSupported) {
                controlPanel.openGLButton.enabled = false
                controlPanel.openGLButton.currentSelection = 0
            }
        }
    }

    MathControl {
        id: mathControl
        width: parent.width
        height: childrenRect.height
        anchors.left: scopeView.left
        anchors.leftMargin: 10
        anchors.top: scopeView.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 1
    }
}