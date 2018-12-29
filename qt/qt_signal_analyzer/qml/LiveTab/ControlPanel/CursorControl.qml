import QtQuick.Controls 2.5
import QtQuick 2.12

import "../../CustomComponents"
import "../../CustomStyle"

Item {
    id: root 

    CollapsibleSection {
            id: cursorHeader
            width: parent.width
            displayText: "Cursor Settings"
            anchors.topMargin: 1
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
                id: cursorButton
                text: "Cursor Target: "
                buttonBorderColor: ScopeSetting.signalBorderColorList[currentSelection]
                buttonTextColor: ScopeSetting.signalColorList[currentSelection]
                items: signalNames
                currentSelection: 0
        }

        CheckBox {
                anchors.left: cursorData.left
                anchors.verticalCenter: cursorButton.verticalCenter
                implicitHeight: 40
                implicitWidth: 110

                id: checkBox

                checked: false


                contentItem: Label {
                    text: "Enabled"
                    id: checkboxText
                    anchors.top: checkBox.top
                    anchors.left: checkBox.indicator.right
                    anchors.right: checkBox.right
                    anchors.bottom: checkBox.bottom

                    topPadding: 0
                    leftPadding: checkBox.spacing
                    rightPadding: checkBox.spacing
                    font.weight: Font.DemiBold
                    verticalAlignment: Text.AlignVCenter

                    color: checkBox.checked ? "#E2E8E7" : "#A7ACAB"

                    background: Rectangle {
                        x: checkBox.indicator.width + 5
                        color: checkBox.hovered ? UIStyle.buttonBgHovered : UIStyle.buttonBgUnhovered
                        border.color: "transparent"
                    }
                }

                indicator: Rectangle {
                    implicitWidth: 20
                    implicitHeight: 20
                    x: checkBox.leftPadding
                    y: parent.height / 2 - height / 2
                    radius: 4
                    color: UIStyle.buttonBgHovered
                    border.color: UIStyle.buttonBgUnhovered

                    Rectangle {
                        anchors.fill: parent
                        radius: 4
                        color: checkBox.checked ? "#D8DEDD" : UIStyle.buttonBgHovered
                        visible: checkBox.checked
                    }
                }

                background: Rectangle {
                        radius: 4
                        border.color: "#E2E8E7"
                        border.width: checkBox.visualFocus ? 2 : 1
                        color: checkBox.hovered ? UIStyle.buttonBgHovered : UIStyle.buttonBgUnhovered
                }

                onClicked: {
                    if(checked) {
                        cursorDisplayChanged(true)
                    } else {
                        cursorDisplayChanged(false)
                    }
                }
            }


        Column {
            id: cursorControl
            anchors.top: cursorButton.bottom
            anchors.topMargin: 0
            width: parent.width / 2
            height: childrenRect.height

            ListModel {
                id: cursorSliderModel

                ListElement{
                    cursorType: "x"
                    cursorIndex: 0
                    text: "Cursor X_A"
                } 
                ListElement{
                    cursorType: "x"
                    cursorIndex: 1
                    text: "Cursor X_B"
                }
                ListElement{
                    cursorType: "y"
                    cursorIndex: 0
                    text: "Cursor Y_A"
                } 
                ListElement{
                    cursorType: "y"
                    cursorIndex: 1
                    text: "Cursor Y_B"
                }
            }

            Repeater {
                model: cursorSliderModel
                SliderWithText {
                    sliderFrom: 0
                    sliderTo: 100
                    sliderDefaultVal: 50
                    sliderText: text
                    sliderTextColor: "#B0B5B4"
                    sliderEnableColor: "#4D7AB5"
                    width: parent.width
                    onSliderMoved: {
                        if(cursorButton.items[cursorButton.currentSelection] !== "None") {
                            cursorType === "x" ? cursorXChanged(cursorIndex, newRange)
                                               : cursorYChanged(cursorButton.currentSelection, cursorIndex, newRange)
                        }
                    }
                }
            }
        }

        Rectangle {
            id: cursorDivider
            anchors.top: cursorButton.bottom
            anchors.topMargin: 0
            anchors.left: cursorControl.right
            anchors.leftMargin: 40
            height: cursorControl.height
            width: 1
            color:"#6D706F"
        }

        Rectangle {
            id: cursorData
            anchors.top: cursorButton.bottom
            anchors.topMargin: 0
            anchors.left: cursorDivider.right
            anchors.leftMargin: 40
            anchors.right: parent.right
            radius: 4
            width: parent.width / 2
            height: childrenRect.height
            color: "transparent"

            Column {
                Label {
                    font.pointSize: 13
                    font.bold: true
                    text: "<font color='#B0B5B4'>X_B - X_A: </font> <font color='#78D1C5'>" + xCursorDif + "</font>"
                }
                Label {
                    font.pointSize: 13
                    font.bold: true
                    text: "<font color='#B0B5B4'>Y_A: </font> <font color='#78D1C5'>" + yCursorA + "</font>"
                }
                Label {
                    font.pointSize: 13
                    font.bold: true
                    text: "<font color='#B0B5B4'>Y_B: </font> <font color='#78D1C5'>" + yCursorB + "</font>"
                }
                Label {
                    font.pointSize: 13
                    font.bold: true
                    text: "<font color='#B0B5B4'>Y_B - Y_A: </font> <font color='#78D1C5'>" + yCursorDif + "</font>"
                }
            }
        }
    }
}
