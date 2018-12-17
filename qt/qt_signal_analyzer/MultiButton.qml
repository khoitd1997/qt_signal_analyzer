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

import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQml 2.12
import QtQuick.Controls.Material 2.4

Item {
    id: button

    property string text: "Option: "
    property variant items: ["first"]
    property int currentSelection: 0
    signal selectionChanged(variant selection)
    signal clicked

    implicitWidth: comboBox.implicitWidth + 5
    implicitHeight: comboBox.implicitHeight + 10

    ComboBox {
        id: comboBox
        width: parent.width
        height: parent.height
        hoverEnabled : true
        model: button.items

        delegate: ItemDelegate {
                width: comboBox.width
                contentItem: Text {
                    text: modelData
                    color: "#21be2b"
                    font: comboBox.font
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
                highlighted: comboBox.highlightedIndex === index
            }

        contentItem: Label {
            leftPadding: 10
            rightPadding: comboBox.indicator.width + comboBox.spacing
            text: button.text + comboBox.displayText
            font: comboBox.font
            color: comboBox.pressed ? "#17a81a" : "#21be2b"
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            radius: 4
            implicitWidth: 120
            implicitHeight: 40
            border.color:  "#17a81a"
            border.width: comboBox.visualFocus ? 2 : 1
            color: comboBox.hovered ? "#616161" : "#424242"
        }

        onActivated: {
            currentSelection = index;
            selectionChanged(button.items[index]);
        }
    }
}
