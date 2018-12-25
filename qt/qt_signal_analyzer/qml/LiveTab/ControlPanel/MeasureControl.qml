import QtQuick.Controls 2.5
import QtQuick 2.12
import QtQml 2.12
import QtQuick.Dialogs 1.3

import "../../CustomComponents"
import "../../CustomStyle"

Item {
    id: root

    CollapsibleSection {
        id: measureHeader
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 0
        displayText: "Measurements and Logger"
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
                            duration: 150 
        }

        PropertyAnimation { id: tabUp2;
                            easing.type: Easing.Linear;
                            target: measureSection;
                            property: "height";
                            from: measureSection.childrenRect.height + 10;
                            to: 0;
                            duration: 150 
        }
        
        id: measureSection
        width: measureHeader.width
        visible: true
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
            property var valueList: [0.0,0.1,0.2,0.3]
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
                                color: ScopeSetting.cellColorList[modelData]
                                font.bold: true
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            Component.onCompleted: {
                                columnRepeater.typeIndex.push(0);
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
                                color: ScopeSetting.cellColorList[modelData]
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
                            color: ScopeSetting.cellColorList[modelData]
                            horizontalAlignment: Text.AlignHCenter
                            text:  Math.round(columnRepeater.valueList[modelData] * 1000) / 1000
                            font.pointSize: 13
                            font.bold: true
                        }

                        CheckBox {
                            width: headerModel.get(3).cellWidth
                            checked: false
                            onClicked: {
                                var tempList = columnRepeater.enabledList;
                                tempList[modelData] = checked;
                                columnRepeater.enabledList = tempList;
                            }
                        }
                    }

                    Rectangle {
                        width: root.width
                        height: 1
                        color: "#6C6F6F"
                    }
                }
            }

            Connections {
                target: dataSource
                onDataProcessDone: {
                    var newItems = dataSource.measureModule.getResult();
                    columnRepeater.valueList = newItems;
                }
            }

            Component.onCompleted: {
                dataSource.measureModule.setGuiSource(columnRepeater)
            }
        }

        Rectangle {
            id: loggerSectionSeparator
            anchors.top: columnRepeater.bottom
            anchors.topMargin: 10
            width: root.width
            height: 0
            color: "#6C6F6F"
        }

        FileDialog {
            visible: false
            id: loggerSaveDialog
            title: "Please choose a csv file to save graph data"
            selectExisting: false
            folder: shortcuts.home
            nameFilters: [ "csv file (*.csv)"]

            onAccepted: {
                loggerButton.elapsedTime = 0;
                var urlStripped = (loggerSaveDialog.fileUrls.toString()+"").replace('file://', '');
                console.log(urlStripped);
                var res = dataSource.loggerModule.switchLogger(true, loggerButton.serieEnabledList, urlStripped, signalNames);
                
                if(res === "") {
                    actionStatusChanged("Logger Started")
                    loggerTimer.restart();
                    loggerButton.isRecording = !loggerButton.isRecording;
                } else {
                        actionStatusChanged("Error: " + res)
                }
            }
        }

        Rectangle {
            id: loggerButton

            property bool isRecording: false
            property int elapsedTime: 0
            property int totalPoint: 0
            property var serieEnabledList: []

            anchors.top: loggerSectionSeparator.bottom
            anchors.topMargin: -6
            anchors.left: parent.left
            anchors.leftMargin: 4
            color: loggerButtonMouse.containsMouse ? UIStyle.buttonBgHovered
                                                   : UIStyle.buttonBgUnhovered
            border.color: loggerButton.isRecording ? "#C02F21" : "#1FE524"
            radius: 5
            width: childrenRect.width + 2
            height: childrenRect.height + 4

            Component.onCompleted: {
                dataSource.loggerModule.setGuiSource(loggerButton);
            }

            Label {
                id: loggerStatusText
                width: 100
                height: Text.paintedHeight
                color: loggerButton.isRecording ? "#C02F21" : "#1FE524"
                font.bold: true
                font.pointSize: 13
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.verticalCenter: parent.verticalCenter

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: loggerButton.isRecording ? "Stop Logger " : "Start Logger "
            }

            Image {
                id: loggerIcon
                anchors.top: parent.top
                anchors.left: loggerStatusText.right
                anchors.topMargin: 4
                anchors.leftMargin: 5

                width: 23
                height: 23
                source: loggerButton.isRecording ? "qrc:///qml/resources/stop-button.png"
                                                 : "qrc:///qml/resources/play-button.png"
            }
            
            MouseArea {
                id: loggerButtonMouse
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    if(!loggerButton.isRecording) {
                        loggerSaveDialog.visible = true;
                    } else {
                        loggerTimer.stop();
                        var res = dataSource.loggerModule.switchLogger(false, [], "", []);

                        if(res === "") {
                            actionStatusChanged("Logger File Saved");
                        } else {
                            actionStatusChanged("Error: " + res);
                        }
                        loggerButton.isRecording = !loggerButton.isRecording;
                    }
                }
            }
        }

        Rectangle {
            id: loggerInfo
            anchors.top: loggerButton.bottom
            anchors.topMargin: 4
            anchors.left: measureSection.left
            height: childrenRect.height
            width: measureSection.width * 0.4
            color: "transparent"

            Timer {
                id: loggerTimer
                property int timerResolution: 250
                running: false
                repeat: true
                interval: timerResolution
                onTriggered: {
                    loggerButton.elapsedTime += timerResolution;
                }
            }

            Label {
                id: timeLabel
                anchors.top: parent.top
                anchors.left: parent.left
                font.bold: true
                font.pointSize: 13
                leftPadding: 8
                color: "#E2E8E7"
                text:"Total Time(ms): " + loggerButton.elapsedTime
            }

            Label {
                font.bold: true
                font.pointSize: 13
                anchors.top: timeLabel.bottom
                anchors.left: parent.left
                leftPadding: 8
                color: "#E2E8E7"
                text:"Total Data Points: " + loggerButton.totalPoint
            }
        }

        Rectangle {
            id: loggerInternalSeparator
            anchors.top: loggerInfo.top
            anchors.left: loggerInfo.right
            anchors.leftMargin: 30

            width: 1
            height: loggerInfo.height
            color: "#6C6F6F"
        }

        Label {
            id: loggerTargetText

            topPadding: -4
            leftPadding: 40

            anchors.left: loggerInternalSeparator.right
            anchors.leftMargin: 20
            anchors.top: loggerSectionSeparator.bottom


            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            color: "#E2E8E7"
            font.bold: true
            font.pointSize: 13
            text: "Logger Target"
        }

        Rectangle {
            id: loggerCheckBoxRect

            anchors.left: loggerTargetText.left
            anchors.top: loggerInternalSeparator.top
            anchors.topMargin: -6
            anchors.right: parent.right
            color: "transparent"

            Grid {
                rows: 2
                columns: 2
                columnSpacing : 60
                rowSpacing: 3

                Repeater {
                    model: signalNames
                    CheckBox {
                        id: loggerCheckBox
                        height: 25
                        checked: false
                        contentItem: Label {
                            anchors.top: loggerCheckBox.top
                            anchors.left: loggerCheckBox.indicator.right
                            anchors.right: loggerCheckBox.right
                            anchors.bottom: loggerCheckBox.bottom
                            verticalAlignment: Text.AlignVCenter
                            topPadding: loggerCheckBox.height / 5 - 2
                            leftPadding: loggerCheckBox.spacing
                            rightPadding: loggerCheckBox.spacing

                            text: modelData
                            font.pointSize: 13
                            font.bold: true
                            color: ScopeSetting.signalColorList[index]
                        }
                        
                        onClicked: {
                            var tempList = loggerButton.serieEnabledList;
                            tempList[index] = checked;
                            loggerButton.serieEnabledList = tempList;
                        }

                        Component.onCompleted: {
                            loggerButton.serieEnabledList.push(false)
                        }
                    }
                }
            }
        }
    }
}
