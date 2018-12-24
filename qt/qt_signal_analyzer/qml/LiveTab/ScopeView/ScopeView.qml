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
import "../../CustomStyle"

ChartView {
    id: chartView
    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark

    signal newCursorData(real xCursorDif, real yCursorA, real yCursorB)

    property bool cursorDataChanged: false

    property bool openGL: true
    property bool openGLSupported: true

    property real oldYOffset: 0
    property real oldXOffset: 0

    property real xCursorDif : 0
    property var yCursorData : [0,0]

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
        dataSource.graphModule.updateSeriesRef(serieList);
        dataSource.start();
    }

    ValueAxis {
        titleText: "Time"
        id: xAxis
        min: 400
        max: 4000

    }

    property var serieList: [lineSeries0, lineSeries1, lineSeries2, lineSeries3]
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

    property var xCursorList: [cursorX0, cursorX1]
    property var cursorXPrevPosition: [0, 0]
    LineSeries {
        id: cursorX0
        XYPoint { x: xAxis.min; y: 1 }
        XYPoint { x: xAxis.min; y: 0 }

        visible: false
        name: "Cursor X_A"
        axisX: xAxis
        axisY: ValueAxis {
            visible: false
            max: 1
            min: 0
        }

        color: "#B52E92"
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: cursorX1
        XYPoint { x: xAxis.max; y: 1 }
        XYPoint { x: xAxis.max; y: 0 }

        visible: false
        name: "Cursor X_B"
        axisX: xAxis
        axisY: ValueAxis {
            visible: false
            max: 1
            min: 0
        }
        color: "#A038B5"
        useOpenGL: chartView.openGL
    }

    property var yCursorList: [cursorY0, cursorY1]
    property var yCursorPrevPosition: [0, 0]
    LineSeries {
        id: cursorY0
        XYPoint { x: 1; y: 1 }
        XYPoint { x: 0; y: 1 }

        visible: false
        name: "Cursor Y_A"
        axisX: ValueAxis {
            visible: false
            max: 1
            min: 0
        }

        axisY: serieList[0].axisY

        color: "#307054"
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: cursorY1
        XYPoint { x: 1; y: 0 }
        XYPoint { x: 0; y: 0 }

        visible: false
        name: "Cursor Y_B"
        axisX: ValueAxis {
            visible: false
            max: 1
            min: 0
        }
        axisY: serieList[0].axisY
        color: "#439D76"
        useOpenGL: chartView.openGL
    }

    Connections {
        target: dataSource
        onDataProcessDone: {
            for(var i = 0; i < serieList.length; ++i) {
                chartView.updateGraph(i);
            }
        }
    }

//    Timer {
//        id: refreshTimer
//        interval: 300
//        running: true
//        repeat: true
//        onTriggered: {
//            dataSource.update();
//            chartView.updateGraph(1);
//            chartView.updateGraph(2);
//            chartView.updateGraph(3);
//        }
//    }

    function saveGraphImage(graphImageUrl) {
        // need to strip due to Qt bugs
        chartView.grabToImage(function(result) {
                        var urlStripped = (graphImageUrl+"").replace('file://', '');
                        result.saveToFile(urlStripped);
                              });
    }

    function updateGraph(id){
        if (serieList[id].visible) {
            var serie = serieList[id]
            xAxis.min += serie.at(serie.count - 1).x + 5 - xAxis.max;
            xAxis.max += serie.at(serie.count - 1).x + 5 - xAxis.max;
        }
        for(var i = 0; i < xCursorList.length; ++i) {
            updateXCursor(i)
        }
        for(var j = 0; j < yCursorList.length; ++j) {
            updateYCursor(j)
        }

        chartView.cursorDataChanged = false
    }

    function changeSeriesName(id, newName) {
        serieList[id].name = newName;
    }

    function changeSeriesDisplay(id, isOn) {
        serieList[id].visible = isOn
    }

    function changeAxisOffset(id, axisName, newOffset){
        if (axisName === "x") {
            xAxis.min += (newOffset-oldXOffset)
            xAxis.max += (newOffset-oldXOffset)
            oldXOffset = newOffset
        } else if(axisName === "y") {
            if(id%2 !=0){
                serieList[id].axisYRight.max += (newOffset-oldYOffset)
                serieList[id].axisYRight.min += (newOffset-oldYOffset)
            } else {
                serieList[id].axisY.max += (newOffset-oldYOffset)
                serieList[id].axisY.min += (newOffset-oldYOffset)
            }
            oldYOffset = newOffset
        }
    }

    function changeAxisRange(id, axisName, newRange){
        var serie = serieList[id]
        if (axisName === "x") {
            xAxis.max += (newRange - (serie.axisX.max - serie.axisX.min))/2
            xAxis.min -= (newRange - (serie.axisX.max - serie.axisX.min))/2

        } else if(axisName === "y") {
            if((id%2) !=0){
                serie.axisYRight.max += (newRange - (serie.axisYRight.max - serie.axisYRight.min))/2.0
                serie.axisYRight.min -= (newRange - (serie.axisYRight.max - serie.axisYRight.min))/2.0
            } else {
                serie.axisY.max += (newRange - (serie.axisY.max - serie.axisY.min))/2.0
                serie.axisY.min -= (newRange - (serie.axisY.max - serie.axisY.min))/2.0
            }
        }
    }

    function updateXCursor(cursorIndex) {
        var cursor = xCursorList[cursorIndex]
        if(cursor.visible === true) {
            changeXCursor(cursorIndex, cursorXPrevPosition[cursorIndex])
        }
    }

    function changeXCursor(cursorIndex, newPosition) {
        var cursor = xCursorList[cursorIndex]
        cursor.remove(1)
        cursor.remove(0)
        cursor.append(xAxis.min + (newPosition/100.0) * (xAxis.max - xAxis.min), cursor.axisY.max)
        cursor.append(xAxis.min + (newPosition/100.0) * (xAxis.max - xAxis.min), cursor.axisY.min)
        cursorXPrevPosition[cursorIndex] = newPosition
        cursor.visible = true
    }

    function updateYCursor(cursorIndex) {
        var cursor = yCursorList[cursorIndex]
        if(cursor.visible === true) {
        cursor.remove(1)
        cursor.remove(0)
        cursor.append(cursor.axisX.max, cursor.axisY.min + (yCursorPrevPosition[cursorIndex]/100.0) * (cursor.axisY.max - cursor.axisY.min))
        cursor.append(cursor.axisX.min, cursor.axisY.min + (yCursorPrevPosition[cursorIndex]/100.0) * (cursor.axisY.max - cursor.axisY.min))
        if(cursor.at(0).y !== chartView.yCursorData[cursorIndex]) {
            chartView.cursorDataChanged = true
        }
        }
    }

    function changeYCursor(serieIndex, cursorIndex, newPosition) {
        var cursor = yCursorList[cursorIndex]
        cursor.remove(1)
        cursor.remove(0)

        if(serieIndex%2 !=0){
            cursor.axisY = serieList[serieIndex].axisYRight
        } else {
            cursor.axisY = serieList[serieIndex].axisY
        }

        cursor.append(cursor.axisX.max, cursor.axisY.min + (newPosition/100.0) * (cursor.axisY.max - cursor.axisY.min))
        cursor.append(cursor.axisX.min, cursor.axisY.min + (newPosition/100.0) * (cursor.axisY.max - cursor.axisY.min))
        yCursorPrevPosition[cursorIndex] = newPosition
        cursor.visible = true
    }

    function cursorSwitch(isOn) {
        for(var i = 0; i < xCursorList.length; ++i) {
            xCursorList[i].visible = isOn
        }
        for(var j = 0; j < yCursorList.length; ++j) {
            yCursorList[j].visible = isOn
        }
    }

    function changeRefreshRate(rate) {
        refreshTimer.interval = 1 / Number(rate) * 1000;
    }

}
