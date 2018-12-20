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
import QtQuick 2.12
import QtQuick.Controls.Material 2.4
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.3

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

    menuBar: MainMenuBar {
        leftPadding: sideBar.width
        onGraphImageRequested: {
            scopeView.saveGraphImage(graphImageUrl)
            bottomBar.actionStatus = "Graph Saved at " + graphImageUrl
        }
    }

    SideBar {
        id: sideBar
        y: main.menuBar.height
        width: 60
        height: main.height
        onTabChanged: {
            changeTab(newTabIndex)
        }
    }

    LiveTab {
            id: liveTab
            width: parent.width
            height: parent.height
            anchors.top: menuBar.bottom
            anchors.right: parent.right
            anchors.leftMargin: sideBar.width + 2
            anchors.left: parent.left
    }

    BottomBar {
        id: bottomBar
    }

    function changeTab(newTabIndex) {
        if(newTabIndex === 0) {
            liveTab.visible = false
        } else if (newTabIndex === 1) {
            liveTab.visible = true
        }
    }
}
