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

import "ScopeView.js" as Scope
import "Cursor.js" as Cursor
import "Axis.js" as Axis

import Qt.analyzer.graphModule 1.0

ChartView {
    id: chartView
    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark

    property bool openGL: true
    property bool openGLSupported: true

    // graph control stuffs
    property real oldYOffset: 0
    property real oldXOffset: 0

    // cursor stuffs
    signal newCursorData(real xCursorDif, real yCursorA, real yCursorB, real yCursorDif)
    property bool cursorDataChanged: false
    property var xCursorData: [0,0]
    property var yCursorData: [0,0]

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

        GraphModule.addSerie(serieList);
        GraphModule.setGuiSource(chartView)
        dataSource.start();
    }

    ValueAxis { id: xAxis; titleText: "Time";  min: 400; max: 4000; }

    property var serieList: [lineSerie0, lineSerie1, lineSerie2, lineSerie3]

    LineSeries {
        id: lineSerie0

        visible: false
        name: "Signal 0"
        color: ScopeSetting.signalColorList[0]
        axisX: xAxis

        axisY: ValueAxis {
            tickCount: 8
            visible: lineSerie0.visible
            labelsColor: lineSerie0.color
            color: lineSerie0.color
            min: -6
            max: 6
        }
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSerie1

        visible: false
        name: "Signal 1"
        color: ScopeSetting.signalColorList[1]
        axisX: xAxis

        axisYRight: ValueAxis {
            tickCount: 8
            visible: lineSerie1.visible
            labelsColor: lineSerie1.color
            color: lineSerie1.color
            min: -6
            max: 6
        }
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSerie2

        visible: false
        name: "Signal 2"
        color: ScopeSetting.signalColorList[2]
        axisX: xAxis

        axisY: ValueAxis {
            tickCount: 8
            visible: lineSerie2.visible
            labelsColor: lineSerie2.color
            color: lineSerie2.color
            min: -6
            max: 6
        }
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: lineSerie3

        visible: false
        name: "Signal 3"
        color: ScopeSetting.signalColorList[3]
        axisX: xAxis

        axisYRight: ValueAxis {
            tickCount: 8
            visible: lineSerie3.visible
            labelsColor: lineSerie3.color
            color: lineSerie3.color
            min: -6
            max: 6
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
        name: "Cursor X_0"
        axisX: xAxis
        axisY: ValueAxis {visible: false; max: 6; min: -6; }

        color: "#B52E92"
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: cursorX1
        XYPoint { x: xAxis.max; y: 1 }
        XYPoint { x: xAxis.min; y: 0 }

        visible: false
        name: "Cursor X_1"
        axisX: xAxis
        axisY: ValueAxis { visible: false; max: 6; min: -6; }
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
        name: "Cursor Y_0"
        axisX: ValueAxis { visible: false; max: 1; min: 0; }
        axisY: serieList[0].axisY

        color: "#307054"
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: cursorY1
        XYPoint { x: 1; y: 0 }
        XYPoint { x: 0; y: 0 }

        visible: false
        name: "Cursor Y_1"
        axisX: ValueAxis { visible: false; max: 1; min: 0; }
        axisY: serieList[0].axisY
        color: "#439D76"
        useOpenGL: chartView.openGL
    }

    property var mathSerieList: [math0, math1]
    LineSeries {
        id: math0
        visible: false
        name: "Math 0"
        axisX: ValueAxis { visible: false; max: xAxis.max; min: xAxis.min; }

        axisY: ValueAxis {visible: false; max: 5; min: -5;}
        color: ScopeSetting.cellColorList[0]
        useOpenGL: chartView.openGL
    }

    LineSeries {
        id: math1
        visible: false
        name: "Math 1"
        axisX: ValueAxis { visible: false; max: xAxis.max; min: xAxis.min; }

        axisY: ValueAxis { visible: false; max: 5; min: -5; }
        color: ScopeSetting.cellColorList[1]
        useOpenGL: chartView.openGL
    }

    function saveGraphImage(graphImageUrl) {
        // need to strip due to Qt bugs
        chartView.grabToImage(function(result) {
                        var urlStripped = (graphImageUrl+"").replace('file://', '');
                        result.saveToFile(urlStripped);
                              });
    }

    function updateGraphs() {
        Axis.scrollXAxis(chartView, xAxis);

        for(var xCursorIndex = 0; xCursorIndex < xCursorList.length; ++xCursorIndex) {
            Cursor.updateXCursor(chartView, xAxis, xCursorIndex);
        }

        for(var yCursorIndex = 0; yCursorIndex < yCursorList.length; ++yCursorIndex) {
            Cursor.updateYCursor(chartView, yCursorIndex);
        }

        if(cursorDataChanged) {
            newCursorData(chartView.xCursorData[1] - chartView.xCursorData[0],
                          chartView.yCursorData[0], chartView.yCursorData[1],
                          chartView.yCursorData[1] - chartView.yCursorData[0]);
        }
        chartView.cursorDataChanged = false;
    }

    function changeAxisOffset(id, axisName, newOffset) {
        Axis.changeAxisOffset(chartView, xAxis, id, axisName, newOffset);
    }

    function changeAxisRange(id, axisName, newRange) {
        Axis.changeAxisRange(chartView, xAxis, id, axisName, newRange);
    }

    function changeXCursor(cursorIndex, newPosition) {
        Cursor.changeXCursor(chartView, xAxis, cursorIndex, newPosition);
    }

    function changeYCursor(serieIndex, cursorIndex, newPosition) {
        Cursor.changeYCursor(chartView, serieIndex, cursorIndex, newPosition);
    }

    function cursorDisplayChanged(isOn) {
        Cursor.cursorDisplayChanged(chartView, isOn);
    }

    function changeRefreshRate(rate) {
        refreshTimer.interval = 1 / Number(rate) * 1000;
    }
}
