import QtQuick.Controls 2.5
import QtQuick 2.12

import "../../CustomComponents"
import "../../CustomStyle"

Item {
    id: root
    property var tableValues: [15, 20, 15, 30]

    CollapsibleSection {
            id: measureHeader
            width: parent.width
            anchors.top: graphControl.bottom
            anchors.topMargin: 10
            displayText: "Measurements"
            onIsClicked: {
                isOn ? tabUp2.running = true : tabDown2.running = true
                isOn ? measureSection.visible = false : measureSection.visible = true
            }
    }

    Rectangle {
        PropertyAnimation { id: tabDown2;
                            easing.type: Easing.Linear	;
                            target: measureSection;
                            property: "height";
                            from: 0;
                            to: measureSection.childrenRect.height + 10;
                            duration: 150 }

        PropertyAnimation { id: tabUp2;
                            easing.type: Easing.Linear;
                            target: measureSection;
                            property: "height";
                            from: measureSection.childrenRect.height + 10;
                            to: 0;
                            duration: 150 }
        id: measureSection
        width: measureHeader.width
        visible: false
        anchors.top: measureHeader.bottom
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
                    ListElement { cellTitle: "Type"; cellWidth: 140}
                    ListElement { cellTitle:  "Source"; cellWidth: 122}
                    ListElement { cellTitle:  "Value"; cellWidth: 30}
                    ListElement { cellTitle:  "Enabled"; cellWidth: 60}
                }

                Repeater {
                    id: headerRepeater
                    model: headerModel
                    Label {
                        font.bold: true
                        color: "#E2E8E7"
                        width: cellWidth
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
            property var typeIndex: []
            property var valueList: [0.0,0.2,0.3,0.4,0.5]
            property var sourceList: []
            property var enabledList: []

            Repeater {
                id: tableRepeater
                model: [0,1,2,3]
                Column  {
                    Row {
                        spacing: headerRow.cellSpacing

                        ComboBox {
                            id: typeBox
                            width: headerModel.get(0).cellWidth
                            model: ["Freq", "Period", "Max", "Min"]
                            contentItem: Text {
                                font.pointSize: 13
                                leftPadding: 10
                                text: typeBox.currentText
                                color: ScopeSetting.signalColorList[sourceBox.currentIndex]
                                font.bold: true
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            Component.onCompleted: {
                                columnRepeater.typeIndex.push(0);
                                columnRepeater.valueList.push(0.0);
                                columnRepeater.sourceList.push(0);
                                columnRepeater.enabledList.push(false);
                            }

                            onActivated: {
                                console.log("Type index changed")
                                columnRepeater.typeIndex[modelData] = typeBox.currentIndex;
                            }
                        }

                        ComboBox {
                            id: sourceBox
                            contentItem: Text {
                                font.pointSize: 13
                                leftPadding: 10
                                text: signalNames[sourceBox.currentIndex]
                                color: ScopeSetting.signalColorList[sourceBox.currentIndex]
                                font.bold: true
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }
                            onActivated: {
                                columnRepeater.sourceList[modelData] = sourceBox.currentIndex;
                            }

                            width: headerModel.get(1).cellWidth
                            model: signalNames
                        }

                        Label {
                            width: headerModel.get(2).cellWidth
                            topPadding: 15
                            color: ScopeSetting.signalColorList[sourceBox.currentIndex]
                            horizontalAlignment: Text.AlignHCenter
                            text:  columnRepeater.valueList[modelData]
                            font.pointSize: 13
                            font.bold: true
                        }

                        CheckBox {
                            width: headerModel.get(3).cellWidth
                            checked: false
                            onClicked: {
                                columnRepeater.enabledList[modelData] = checked
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

            Connections {
                target: dataSource
                onDataProcessDone: {
                    columnRepeater.valueList = newItems;
                }
            }

            Component.onCompleted: {
                dataSource.measureModule.setGuiSource(columnRepeater)
            }
        }
    }
}
