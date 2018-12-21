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
import "."

ChartView {
    id: chartView
    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark
    property bool openGL: true
    property bool openGLSupported: true

    property real oldYOffset: 0
    property real oldXOffset: 0

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
        titleText: "Time"
        id: xAxis
        min: 400
        max: 4000
    }

    LineSeries {
        id: lineSeries0
        visible: false
        name: "Signal 0"
        color: ScopeSetting.signalColorList[0]
        axisX: xAxis
        axisY: ValueAxis {
            tickCount: 8
            visible: lineSeries0.visible
            labelsColor: lineSeries0.color
            color: lineSeries0.color
            min: -5
            max: 5
        }
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeries1
        visible: false
        name: "Signal 1"
        color: ScopeSetting.signalColorList[1]
        axisX: xAxis
        axisYRight: ValueAxis {
            tickCount: 8
            visible: lineSeries1.visible
            labelsColor: lineSeries1.color
            color: lineSeries1.color
            min: -5
            max: 5
        }
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeries2
        visible: false
        name: "Signal 2"
        axisX: xAxis
        color: ScopeSetting.signalColorList[2]
        axisY: ValueAxis {
            tickCount: 8
            visible: lineSeries2.visible
            labelsColor: lineSeries2.color
            color: lineSeries2.color
            min: -5
            max: 5
        }
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSeries3
        visible: true
        name: "Signal 3"
        axisX: xAxis
        color: ScopeSetting.signalColorList[3]
        axisYRight: ValueAxis {
            tickCount: 8
            visible: lineSeries3.visible
            labelsColor: lineSeries3.color
            color: lineSeries3.color
            min: -5
            max: 5
        }
        useOpenGL: chartView.openGL
    }

    LineSeries {
        XYPoint { x: xAxis.min; y: 2 }
        XYPoint { x: xAxis.max; y: 2 }

        style: Qt.DotLine
        visible: true
        name: "Cursor 1"
        axisX: xAxis
        color: "#78D1C5"
        axisYRight: lineSeries1.axisYRight
        useOpenGL: chartView.openGL
    }

    LineSeries {
        XYPoint { x: xAxis.min; y: 2 }
        XYPoint { x: xAxis.max; y: 2 }

        style: Qt.DotLine
        visible: true
        name: "Cursor 1"
        axisX: xAxis
        color: "#78D1C5"
        axisYRight: lineSeries1.axisYRight
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

    function saveGraphImage(graphImageUrl) {
        // need to strip due to Qt bugs
        chartView.grabToImage(function(result) {
                        var urlStripped = (graphImageUrl+"").replace('file://', '');
                        result.saveToFile(urlStripped);
                              });
    }

    function updateGraph(id){
        if (chartView.series(id).visible) {
            dataSource.update(chartView.series(id));
            xAxis.min += chartView.series(id).at(chartView.series(id).count - 1).x + 5 - xAxis.max;
            xAxis.max += chartView.series(id).at(chartView.series(id).count - 1).x + 5 - xAxis.max;
            chartView.series(4).remove(1)
            chartView.series(4).remove(0)
            chartView.series(4).append(xAxis.min, 2)
            chartView.series(4).append(xAxis.max, 2)
            chartView.series(4).axisYRight = lineSeries1.axisYRight
        }
    }

    function changeSeriesName(id, newName) {
        chartView.series(id).name = newName;
    }

    function changeSeriesDisplay(id, isOn) {
        chartView.series(id).visible = isOn
    }

    function changeAxisOffset(id, axisName, newOffset){
        if (axisName === "x") {
            xAxis.min += (newOffset-oldXOffset)
            xAxis.max += (newOffset-oldXOffset)
            oldXOffset = newOffset
        } else if(axisName === "y") {
            if(id%2 !=0){
                chartView.series(id).axisYRight.max += (newOffset-oldYOffset)
                chartView.series(id).axisYRight.min += (newOffset-oldYOffset)
            } else {
                chartView.series(id).axisY.max += (newOffset-oldYOffset)
                chartView.series(id).axisY.min += (newOffset-oldYOffset)
            }
            oldYOffset = newOffset
        }
    }

    function changeAxisRange(id, axisName, newRange){
        if (axisName === "x") {
            xAxis.max += (newRange -
                            (chartView.series(id).axisX.max - chartView.series(id).axisX.min))/2

            xAxis.min -= (newRange -
                            (chartView.series(id).axisX.max - chartView.series(id).axisX.min))/2

        } else if(axisName === "y") {
            if((id%2) !=0){
                chartView.series(id).axisYRight.max += (newRange
                                                        - (chartView.series(id).axisYRight.max
                                                        - chartView.series(id).axisYRight.min))/2.0

                chartView.series(id).axisYRight.min -= (newRange
                                                        - (chartView.series(id).axisYRight.max
                                                        - chartView.series(id).axisYRight.min))/2.0
            } else {
                chartView.series(id).axisY.max += (newRange
                                                    - (chartView.series(id).axisY.max
                                                    - chartView.series(id).axisY.min))/2.0

                chartView.series(id).axisY.min -= (newRange
                                                    - (chartView.series(id).axisY.max
                                                    - chartView.series(id).axisY.min))/2.0
            }
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
