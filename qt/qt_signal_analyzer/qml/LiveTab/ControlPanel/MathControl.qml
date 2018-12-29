import QtQuick 2.1
import QtQuick.Controls 2.5
import QtQuick 2.12

import "../../CustomComponents"
import "../../CustomStyle"

import Qt.analyzer.mathModule 1.0

Item {
    id: root

    CollapsibleSection {
            id: mathHeader
            width: parent.width
            anchors.topMargin: 10
            displayText: "Math"
            onIsClicked: {
                isOn ? tabUp2.running = true : tabDown2.running = true
                isOn ? mathSection.visible = false : mathSection.visible = true
            }
    }

    Rectangle {
        PropertyAnimation { id: tabDown2;
                            easing.type: Easing.Linear	;
                            target: mathSection;
                            property: "height";
                            from: 0;
                            to: mathSection.childrenRect.height + 10;
                            duration: 150 
        }

        PropertyAnimation { id: tabUp2;
                            easing.type: Easing.Linear;
                            target: mathSection;
                            property: "height";
                            from: mathSection.childrenRect.height + 10;
                            to: 0;
                            duration: 150 
        }
        
        id: mathSection
        width: mathHeader.width
        visible: true
        anchors.top: mathHeader.bottom
        color: "#3A3A3A"

        Rectangle {
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: parent.left

            id: tableHeader
            color: "#6C6F6F"
            height: childrenRect.height
            width: parent.width

            Row {
                id: headerRow
                property int cellSpacing: 70
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 5
                spacing: cellSpacing

                ListModel {
                    id: headerModel
                    ListElement { cellTitle: "Operation"; cellWidth: 110; lPadding: -15}
                    ListElement { cellTitle:  "A"; cellWidth: 122; lPadding: 0}
                    ListElement { cellTitle:  "B"; cellWidth: 122; lPadding: 0}
                    ListElement { cellTitle:  "Enabled"; cellWidth: 40; lPadding: 0}
                }

                Repeater {
                    id: headerRepeater
                    model: headerModel
                    Label {
                        font.bold: true
                        color: "#E2E8E7"
                        width: cellWidth
                        leftPadding: lPadding
                        horizontalAlignment: Text.AlignHCenter
                        text: cellTitle
                    }
                }
            }
        }

        Column {
            id: columnRepeater
            anchors.top: tableHeader.bottom
            width: parent.width
            height: childrenRect.height

            signal mathChanged(int slotIndex, int newType, int newASource,
                               int newBSource, bool isEnabled)

            property var operationIndex: []
            property var valueList: [0.0,0.1,0.2,0.3]
            property var sourceAList: []
            property var sourceBList: []
            property var enabledList: []

            ListModel {
                id: operationTypeList
                ListElement { name: "A-B"; hasTwoOperand: true }
                ListElement { name: "A+B"; hasTwoOperand: true }
                ListElement { name: "Abs(A)"; hasTwoOperand: false }
                ListElement { name: "log(A)"; hasTwoOperand: false }
            }

            Repeater {
                id: tableRepeater
                model: [0,1]
                Column  {
                    Row {
                        spacing: headerRow.cellSpacing

                        ComboBox {
                            id: typeBox
                            width: headerModel.get(0).cellWidth
                            model: operationTypeList
                            textRole: "name"
                            contentItem: Text {
                                font.pointSize: 13
                                leftPadding: 0
                                text: typeBox.currentText
                                color: ScopeSetting.cellColorList[modelData]
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
//                                elide: Text.ElideMiddle
                            }

                            Component.onCompleted: {
                                columnRepeater.operationIndex.push(0);
                                columnRepeater.sourceAList.push(0);
                                columnRepeater.sourceBList.push(0);
                                columnRepeater.enabledList.push(false);
                            }

                            onActivated: {
                                console.log("Math Type index changed")
//                                columnRepeater.operationIndex[modelData] = typeBox.currentIndex;
                                if(mathCheckBox.checked) {
                                columnRepeater.mathChanged(modelData, typeBox.currentIndex, sourceABox.currentIndex,
                                                sourceBBox.currentIndex, mathCheckBox.checked);
                                }

                            }
                        }

                        ComboBox {
                            id: sourceABox
                            contentItem: Text {
                                font.pointSize: 13
                                leftPadding: 10
                                text: signalNames[sourceABox.currentIndex]
                                color: ScopeSetting.cellColorList[modelData]
                                font.bold: true
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }
                            onActivated: {
                                console.log("Source A changed")
//                                columnRepeater.sourceList[modelData] = sourceABox.currentIndex;
                                if(mathCheckBox.checked) {
                                    columnRepeater.mathChanged(modelData, typeBox.currentIndex, sourceABox.currentIndex,
                                                sourceBBox.currentIndex, mathCheckBox.checked);
                                }

                            }

                            width: headerModel.get(1).cellWidth
                            model: signalNames
                        }

                        ComboBox {
                            id: sourceBBox
                            contentItem: Text {
                                font.pointSize: 13
                                leftPadding: 10
                                text: sourceBBox.currentText
                                color: ScopeSetting.cellColorList[modelData]
                                font.bold: true
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }
                                onActivated: {
                                    console.log("Source B changed")
//                                    columnRepeater.sourceList[modelData] = sourceBBox.currentIndex;
                                    if(mathCheckBox.checked) {
                                        columnRepeater.mathChanged(modelData, typeBox.currentIndex, sourceABox.currentIndex,
                                                    sourceBBox.currentIndex, mathCheckBox.checked);
                                    }
                                }

                                width: headerModel.get(2).cellWidth
                                model: operationTypeList.get(typeBox.currentIndex).hasTwoOperand ? signalNames
                                                                                             : ["-"]
                        }

                        CheckBox {
                            id: mathCheckBox
                            width: headerModel.get(3).cellWidth
                            checked: false
                            onClicked: {
                                console.log("Math checked changed")
                                columnRepeater.mathChanged(modelData, typeBox.currentIndex, sourceABox.currentIndex,
                                            sourceBBox.currentIndex, mathCheckBox.checked)
                            }
                        }
                    }

                    Rectangle {
                        width: root.width
                        height: index < (tableRepeater.count - 1) ? 1 : 0
                        color: "#6C6F6F"
                    }
                }
            }

            Component.onCompleted: {
                MathModule.setGuiSource(columnRepeater)
            }
        }
    }
}
