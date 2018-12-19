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
import QtCharts 2.1

ChartView {
    id: chartView
    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark
    property bool openGL: true
    property bool openGLSupported: true
    onOpenGLChanged: {
        if (openGLSupported) {
            series("Signal 1").useOpenGL = openGL;
            series("Signal 2").useOpenGL = openGL;
        }
    }
    Component.onCompleted: {
        if (!series("Signal 0").useOpenGL) {
            openGLSupported = false
            openGL = false
        }
    }

    ValueAxis {
        id: timeAxis
        min: 400
        max: 4000
    }

    LineSeries {
        id: lineSeries0
        name: "Signal 0"
        axisX: timeAxis
        axisY: ValueAxis {
            min: 0
            max: 5
        }
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeries1
        visible: true
        name: "Signal 1"
        axisX: timeAxis

        axisYRight: ValueAxis {
            min: 0
            max: 5
        }
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeries2
        name: "Signal 2"
        axisX: timeAxis
        axisY: ValueAxis {
            min: 0
            max: 1024
        }
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeries3
        visible: true
        name: "Signal 3"
        axisX: timeAxis
        axisYRight: ValueAxis {
            min: 0
            max: 1024
        }
        useOpenGL: chartView.openGL
    }

    Timer {
        id: refreshTimer
        interval: 300
        running: true
        repeat: true
        onTriggered: {
            chartView.updateGraph(0);
            chartView.updateGraph(1);
            chartView.updateGraph(2);
            chartView.updateGraph(3);
        }
    }


    function updateGraph(id){
        if (chartView.series(id).visible) {
            dataSource.update(chartView.series(id));
            timeAxis.min += chartView.series(id).at(chartView.series(id).count - 1).x + 5 - timeAxis.max;
            timeAxis.max += chartView.series(id).at(chartView.series(id).count - 1).x + 5 - timeAxis.max;
        }
    }

    function changeSeriesName(id, newName) {
        chartView.series(id).name = newName;
    }

    function changeSeriesDisplay(id, isOn) {
        chartView.series(id).visible = isOn
    }

    function changeAxis(id, axisName, newRange){
        if (axisName === "x") {
            timeAxis.max += (newRange - 
                            (chartView.series(id).axisX.max - chartView.series(id).axisX.min))/2
            
            timeAxis.min -= (newRange - 
                            (chartView.series(id).axisX.max - chartView.series(id).axisX.min))/2

        } else if(axisName === "y") {
            chartView.series(id).axisY.max += (newRange 
                                              - (chartView.series(id).axisY.max 
                                              - chartView.series(id).axisY.min))/2
            
            chartView.series(id).axisY.min -= (newRange 
                                              - (chartView.series(id).axisY.max 
                                              - chartView.series(id).axisY.min))/2
        }
    }

    function createAxis(min, max) {
        // The following creates a ValueAxis object that can be then set as a x or y axis for a series
        return Qt.createQmlObject("import QtQuick 2.0; import QtCharts 2.0; ValueAxis { min: "
                                  + min + "; max: " + max + " }", chartView);
    }

    function setAnimations(enabled) {
        if (enabled)
            chartView.animationOptions = ChartView.SeriesAnimations;
        else
            chartView.animationOptions = ChartView.NoAnimation;
    }

    function changeRefreshRate(rate) {
        refreshTimer.interval = 1 / Number(rate) * 1000;
    }
}
