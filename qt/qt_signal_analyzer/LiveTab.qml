import QtQuick 2.12
import QtQuick.Controls 2.4

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
    }

    ScopeView {
        id: scopeView
        width: parent.width - controlPanel.width
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
}
