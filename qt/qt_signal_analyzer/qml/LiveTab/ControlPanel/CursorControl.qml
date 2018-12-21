import QtQuick.Controls 2.5
import QtQuick 2.12

import "../../CustomComponents"
import "../../CustomStyle"

Item {
    CollapsibleSection {
            id: cursorHeader
            width: parent.width
            displayText: "Cursor Settings"
            anchors.topMargin: 10
            anchors.top: parent.top
            onIsClicked: {
                isOn ? tabUp3.running = true : tabDown3.running = true
                isOn ? cursorSection.visible = false : cursorSection.visible = true
            }
    }

    Rectangle {
        PropertyAnimation { id: tabDown3;
                            easing.type: Easing.Linear	;
                            target: cursorSection;
                            property: "height";
                            from: 0;
                            to: cursorSection.childrenRect.height + 10;
                            duration: 150 }

        PropertyAnimation { id: tabUp3;
                            easing.type: Easing.Linear;
                            target: cursorSection;
                            property: "height";
                            from: cursorSection.childrenRect.height + 10;
                            to: 0;
                            duration: 150 }
        id: cursorSection
        width: cursorHeader.width
        visible: false
        anchors.top: cursorHeader.bottom
        color: "#3A3A3A"

        MultiButton {
                anchors.top: parent.top
                id: currentCursorButton
                text: "Cursor Target: "
                buttonBorderColor: ScopeSetting.signalBorderColorList[currentSelection]
                buttonTextColor: ScopeSetting.signalColorList[currentSelection]
                items: ["Signal 0", "Signal 1", "Signal 2", "Signal 3"]
                currentSelection: 0
        }


        Column {
            id: cursorControl
            anchors.top: currentCursorButton.bottom
            anchors.topMargin: 0
            width: parent.width / 2
            height: childrenRect.height


            Repeater {
                model: ["Cursor X_A", "Cursor X_B", "Cursor Y_A", "Cursor Y_B"]
                SliderWithText {
                    sliderFrom: 0
                    sliderTo: 100
                    sliderDefaultVal: 50
                    sliderText: modelData
                    sliderTextColor: "#B0B5B4"
                    sliderEnableColor: "#4D7AB5"
                    width: parent.width
                    onSliderMoved: {
                    }
                }
            }
        }

        Rectangle {
            id: cursorDivider
            anchors.top: currentCursorButton.bottom
            anchors.topMargin: 0
            anchors.left: cursorControl.right
            anchors.leftMargin: 40
            height: cursorControl.height
            width: 1
            color:"#6D706F"
        }

        Rectangle {
            id: cursorData
            anchors.top: currentCursorButton.bottom
            anchors.topMargin: 0
            anchors.left: cursorDivider.right
            anchors.leftMargin: 40
            anchors.right: parent.right
            radius: 4
            width: parent.width / 2
            height: childrenRect.height
            color: "transparent"
            Rectangle {
                Label {
                    font.pointSize: 13
                    font.bold: true
                    text: "<font color='#B0B5B4'>X_B - X_A: </font> <font color='#78D1C5'>50</font>"
                }
            }
        }
    }
}
