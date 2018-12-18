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

Column {
    id: colLayout
    spacing: 5
    Layout.fillHeight: true
    height: (checkBoxColumn.height + gridButton.height) * 1.1

    property alias antialiasButton: antialiasButton
    signal animationsEnabled(bool enabled)
    signal refreshRateChanged(variant rate);
    signal signalSourceChanged(string source, int signalCount, int sampleCount);
    signal antialiasingEnabled(bool enabled)

    // graph chooser section
    signal seriesNameChanged(int id, string newName)
    signal seriesDisplayChanged(int id, bool isOn)

    ButtonGroup {
        buttons: checkBoxColumn.children
    }

        Layout.topMargin: 10
        Row  {
            width: parent.width
            id: checkBoxColumn
            spacing: 20
            Repeater {
                model: 4
                CheckboxButton {
                    numberID: index
                    text: "Signal " + index
                    isChecked: true
                    onNameChanged: {
                        seriesNameChanged(numberID, newName)
                    }
                    onCheckChanged: {
                        seriesDisplayChanged(numberID, checkStatus)
                    }
                }
            }
        }

        Grid {
            id: gridButton
            spacing: 20
            columns: 3
            width: parent.width
            MultiButton {
                id: signalSourceButton
                text: "Source: "
                items: ["sin", "linear"]
                currentSelection: 0
                onSelectionChanged: signalSourceChanged(
                                        selection,
                                        5,
                                        sampleCountButton.item[sampleCountButton.currentSelection]
                                    );
        }

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
