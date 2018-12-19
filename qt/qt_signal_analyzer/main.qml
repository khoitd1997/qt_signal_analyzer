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

import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick 2.12
import QtQuick.Controls.Material 2.4


//![1]
ApplicationWindow {
    Material.theme: Material.Dark
    Material.accent: Material.Teal

    id: main

    // specify width and height for when minimized
    width: 600
    height: 400

    visibility: Window.Maximized
    flags: Qt.Window | Qt. WindowTitleHint | Qt.WindowMaximizeButtonHint | Qt.WindowCloseButtonHint | Qt.WindowMinimizeButtonHint
    title: "Qt Signal Analyzer"


    color: "#272727"
    Material.foreground: Material.LightBlue

    ControlPanel {
        id: controlPanel
        anchors.topMargin: 10
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: parent.width / 3

//        onSignalSourceChanged: {
//            if (source == "sin")
//                dataSource.generateData(0, signalCount, sampleCount);
//            else
//                dataSource.generateData(1, signalCount, sampleCount);
//            scopeView.axisX().max = sampleCount;
//        }
        onRefreshRateChanged: scopeView.changeRefreshRate(rate);
        onAntialiasingEnabled: scopeView.antialiasing = enabled;
        onSeriesNameChanged: scopeView.changeSeriesName(id, newName);
        onSeriesDisplayChanged: scopeView.changeSeriesDisplay(id, isOn);
    }

    ScopeView {
        id: scopeView
        width: parent.width - controlPanel.width
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: controlPanel.right
        height: parent.height * 0.8

        onOpenGLSupportedChanged: {
            if (!openGLSupported) {
                controlPanel.openGLButton.enabled = false
                controlPanel.openGLButton.currentSelection = 0
            }
        }
    }
}
