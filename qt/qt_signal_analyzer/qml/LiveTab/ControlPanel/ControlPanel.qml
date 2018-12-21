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

import "../../CustomComponents"
import "../../CustomStyle"

Item {
    id: colLayout
    Layout.fillHeight: true
//    height:  graphControl.height
    width: parent.width

//    property alias antialiasButton: antialiasButton
    signal animationsEnabled(bool enabled)
    signal refreshRateChanged(variant rate);
    signal antialiasingEnabled(bool enabled)

    // graph chooser section
    signal seriesNameChanged(int id, string newName)
    signal seriesDisplayChanged(int id, bool isOn)
    signal axisRangeChanged(int id, string axisName, real newRange)
    signal axisOffsetChanged(int id, string axisName, real newOffset)

    GraphControl {
        id: graphControl
        width: parent.width
        height: childrenRect.height
        anchors.top: parent.top
    }

    CollapsibleSection {
            id: sectionHeader2
            width: parent.width
            anchors.top: graphControl.bottom
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
                            property: "height";
                            from: 0;
                            to: section2.childrenRect.height + 10;
                            duration: 150 }

        PropertyAnimation { id: tabUp2;
                            easing.type: Easing.Linear;
                            target: section2;
                            property: "height";
                            from: section2.childrenRect.height + 10;
                            to: 0;
                            duration: 150 }
        id: section2
        width: sectionHeader2.width
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

    CursorControl {
        width: parent.width
        anchors.top: section2.bottom
        anchors.right: parent.right
    }
}
