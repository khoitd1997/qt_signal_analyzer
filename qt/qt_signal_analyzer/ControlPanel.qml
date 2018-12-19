/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.5
import QtQuick 2.12
import QtQuick.Controls.Material 2.4
import "."

Item {
    id: colLayout
    Layout.fillHeight: true
    height:  section1.height
    width: parent.width

//    property alias antialiasButton: antialiasButton
    signal animationsEnabled(bool enabled)
    signal refreshRateChanged(variant rate);
    signal antialiasingEnabled(bool enabled)

    // graph chooser section
    signal seriesNameChanged(int id, string newName)
    signal seriesDisplayChanged(int id, bool isOn)
    signal axisChanged(int id, string axisName, real newRange)

//    ButtonGroup {
//        buttons: checkBoxColumn.children
//    }
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

            spacing: parent.width / 12
            topPadding: 10
            Repeater {
                model: 4
                CheckboxButton {
                    numberID: index
                    text: "Signal " + index
                    isChecked: true
                    onNameChanged: {
                        seriesNameChanged(numberID, newName)
                        var newItems = currentSignalButton.items
                        newItems[numberID] = newName
                        currentSignalButton.items = newItems
                    }
                    onCheckChanged: {
                        seriesDisplayChanged(numberID, checkStatus)
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
                    items: ["Signal 0", "Signal 1", "Signal 2", "Signal 3"]
                    currentSelection: 0
            }

            SliderWithText {
                id: xSlider
                sliderFrom: 1
                sliderTo: 50000
                sliderDefaultVal: 25000
                sliderText: "Change Time Scaling:"
                width: section1.width
                onSliderMoved: {
                        axisChanged(currentSignalButton.currentSelection, "x", newRange)
                }
            }
        
            SliderWithText {
                anchors.top: xSlider.bottom
                sliderText: "Change Voltage Scaling:"
                sliderFrom: 1
                sliderTo: 5
                sliderDefaultVal: 4
                width: section1.width
                onSliderMoved: {
                        axisChanged(currentSignalButton.currentSelection, "y", newRange)
                }
            }
        }
    }

    CollapsibleSection {
            id: sectionHeader2
            width: parent.width
            anchors.top: section1.bottom
            anchors.topMargin: 10
            displayText: "Signal Display Settings"
            onIsClicked: {
                isOn ? tabUp2.running = true : tabDown2.running = true
            }
    }

    Rectangle {
        PropertyAnimation { id: tabDown2;
                            easing.type: Easing.Linear	;
                            target: section2;
                            property: "visible";
                            from: false;
                            to: true;
                            duration: 150 }

        PropertyAnimation { id: tabUp2;
                            easing.type: Easing.Linear;
                            target: section2;
                            property: "visible";
                            from: true;
                            to: false;
                            duration: 150 }
        id: section2
        width: sectionHeader1.width
        visible: false
        anchors.top: sectionHeader2.bottom
        color: "#3A3A3A"

        Grid {
        MultiButton {
            id: sampleCountButton
            text: "Samples: "
            items: ["6", "128", "1024", "10000"]
            currentSelection: 2
            onSelectionChanged: signalSourceChanged(
                                    signalSourceButton.item[signalSourceButton.currentSelection],
                                    5,
                                    selection
                                );
        }
        
        MultiButton {
            text: "Refresh Rate: "
            items: ["1", "24", "60"]
            currentSelection: 2
            onSelectionChanged: refreshRateChanged(items[currentSelection]);
        }

        MultiButton {
            id: antialiasButton
            text: "Antialias: "
            items: ["OFF", "ON"]
            enabled: true
            currentSelection: 0
            onSelectionChanged: antialiasingEnabled(currentSelection == 1);
            }
        }
    }
}
