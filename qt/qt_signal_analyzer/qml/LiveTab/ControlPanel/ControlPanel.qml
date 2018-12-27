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
    width: parent.width

    property var signalNames: ["Signal 0", "Signal 1", "Signal 2", "Signal 3"]

    property real yCursorA : 0

    //    property alias antialiasButton: antialiasButton
    signal animationsEnabled(bool enabled)
    signal refreshRateChanged(variant rate);
    signal antialiasingEnabled(bool enabled)

    // cursor control
    signal xCursorChanged(int cursorIndex, real newPosition)
    signal yCursorChanged(int serieIndex, int cursorIndex, real newPosition)
    signal cursorSwitch(bool isOn)

    GraphControl {
        id: graphControl
        width: parent.width
        height: childrenRect.height
        anchors.top: parent.top
    }

    MeasureControl {
        id: measureControl
        width: parent.width
        height: childrenRect.height
        anchors.top: graphControl.bottom
        anchors.right: parent.right
        anchors.topMargin: 1
    }

    CursorControl {
        id: cursorControl
        width: parent.width
        height: childrenRect.height
        anchors.top: measureControl.bottom
        anchors.topMargin: 0
        anchors.right: parent.right
    }

    MathControl {
        id: mathControl
        width: parent.width
        height: childrenRect.height
        anchors.top: cursorControl.bottom
        anchors.topMargin: 2
        anchors.right: parent.right
    }
}
