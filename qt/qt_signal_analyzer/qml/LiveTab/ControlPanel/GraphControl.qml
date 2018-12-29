import QtQuick 2.1
import QtQuick.Controls 2.5
import QtQuick 2.12

import "../../CustomComponents"
import "../../CustomStyle"

import Qt.analyzer.graphModule 1.0


Item {
    id: root

    CollapsibleSection {
            id: sectionHeader1
            width: parent.width
            displayText: "Graph Settings"
            anchors.top: parent.top
            onIsClicked: {
                isOn ? tabUp1.running = true : tabDown1.running = true
                isOn ? section1.visible = false : section1.visible = true
            }
    }

    Rectangle {
        PropertyAnimation { id: tabDown1;
                            easing.type: Easing.Linear	;
                            target: section1;
                            property: "height";
                            from: 0;
                            to: section1.childrenRect.height + 10;
                            duration: 150 }

        PropertyAnimation { id: tabUp1;
                            easing.type: Easing.Linear;
                            target: section1;
                            property: "height";
                            from: section1.childrenRect.height + 10;
                            to: 0;
                            duration: 150 }
        id: section1
        width: sectionHeader1.width
        height: 0
        visible: false
        anchors.top: sectionHeader1.bottom
        color: "#3A3A3A"


        Row  {
            width: parent.width
            id: checkBoxColumn

            spacing: parent.width / 14
            topPadding: 10
            Repeater {
                model: ScopeSetting.signalColorList
                CheckboxButton {
                    numberID: index
                    text: "Signal " + index
                    buttonTextColor: modelData
                    buttonBorderColor: ScopeSetting.signalBorderColorList[index]
                    onNameChanged: {
                        GraphModule.changeSerieName(numberID, newName);
                        var newItems = signalNames;
                        newItems[numberID] = newName;
                        signalNames = newItems;
                    }
                    onCheckChanged: {
                        GraphModule.changeSerieDisplay(numberID, checkStatus);
                    }
                }
            }
        }

        Rectangle {
            anchors.top: checkBoxColumn.bottom
            anchors.topMargin: 10
            radius: 4
            width: childrenRect.width
            height: childrenRect.height
            color: "transparent"

            MultiButton {
                    anchors.top: parent.top
                    id: currentSignalButton
                    text: "Selected Signal: "
                    buttonBorderColor: ScopeSetting.signalBorderColorList[currentSelection]
                    buttonTextColor: ScopeSetting.signalColorList[currentSelection]
                    items: signalNames
                    currentSelection: 0
            }

            ListModel {
                id: sliderGraphModel
                ListElement {
                sliderFromMod: 1
                sliderToMod: 50000
                sliderDefaultValMod: 25000
                sliderType: "scaling"
                sliderTextMod: "Change X Scaling:"
                sliderTextColorMod: "#B0B5B4"
                axisMod: "x"
                }
                
                ListElement {
                sliderTextMod: "Change X Offset: "
                sliderType: "offset"
                sliderFromMod: -20000
                sliderToMod: 20000
                sliderDefaultValMod: 0
                sliderTextColorMod: "#B0B5B4"
                axisMod: "x"
                }
                
                ListElement {
                sliderTextMod: "Change Y Scaling:"
                sliderType: "scaling"
                sliderFromMod: 0.1
                sliderToMod: 10
                sliderDefaultValMod: 4
                sliderTextColorMod: "#B0B5B4"
                axisMod: "y"
                }
                
                ListElement {
                sliderTextMod: "Change Y Offset: "
                sliderType: "offset"
                sliderFromMod: -10
                sliderToMod: 10
                sliderDefaultValMod: 0
                sliderTextColorMod: "#B0B5B4"
                axisMod: "y"
                }
            }
            Column {
//                id: cursorControl
                anchors.top: currentSignalButton.bottom
                anchors.topMargin: 0

            Repeater {
                model: sliderGraphModel
                SliderWithText {
                    sliderFrom: sliderFromMod
                    sliderTo: sliderToMod
                    sliderDefaultVal: sliderDefaultValMod
                    sliderText: sliderTextMod
                    sliderTextColor: sliderTextColorMod
                    width: section1.width
                    onSliderMoved: {
                        if(sliderType === "scaling"){
                            axisRangeChanged(currentSignalButton.currentSelection, axisMod, newRange);
                        } else {
                            axisOffsetChanged(currentSignalButton.currentSelection, axisMod, newRange);
                        }
                    }
                }
            }
            }
       }
    }
}
