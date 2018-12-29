// function test(chartViewObj) {
//     console.log("js test:" + scopeVar.global1);
// }

function removeCursor(cursor) {
    cursor.remove(1);
    cursor.remove(0);
}

function updateXCursor(chartView, xAxis, cursorIndex) {
    var cursor = chartView.xCursorList[cursorIndex]
    if (cursor.visible === true) {
        changeXCursor(chartView, xAxis, cursorIndex, cursorXPrevPosition[cursorIndex])
    }
}

function changeXCursor(chartView, xAxis, cursorIndex, newPosition) {
    var cursor = chartView.xCursorList[cursorIndex]
    var newCursorPosition = xAxis.min + (newPosition / 100.0) * (xAxis.max - xAxis.min);
    removeCursor(cursor);

    cursor.append(newCursorPosition, cursor.axisY.max);
    cursor.append(newCursorPosition, cursor.axisY.min);
    chartView.cursorXPrevPosition[cursorIndex] = newPosition;

    if (chartView.xCursorData[cursorIndex] !== newCursorPosition) {
        chartView.cursorDataChanged = true;
        chartView.xCursorData[cursorIndex] = newCursorPosition;
    }
    cursor.visible = true;
}

function updateYCursor(chartView, cursorIndex) {
    var cursor = chartView.yCursorList[cursorIndex];
    if (cursor.visible === true) {
        var newCursorPosition = cursor.axisY.min +
            (chartView.yCursorPrevPosition[cursorIndex] / 100.0)
            * (cursor.axisY.max - cursor.axisY.min);

        removeCursor(cursor);

        cursor.append(cursor.axisX.max, newCursorPosition);
        cursor.append(cursor.axisX.min, newCursorPosition);

        if (newCursorPosition !== chartView.yCursorData[cursorIndex]) {
            chartView.yCursorData[cursorIndex] = newCursorPosition;
            chartView.cursorDataChanged = true;
        }
    }
}

function changeYCursor(chartView, serieIndex, cursorIndex, newPosition) {
    var cursor = chartView.yCursorList[cursorIndex];
    removeCursor(cursor);

    if (serieIndex % 2 != 0) {
        cursor.axisY = chartView.serieList[serieIndex].axisYRight;
    } else {
        cursor.axisY = chartView.serieList[serieIndex].axisY;
    }

    var newCursorPosition = cursor.axisY.min + (newPosition / 100.0) * (cursor.axisY.max - cursor.axisY.min);

    cursor.append(cursor.axisX.max, newCursorPosition);
    cursor.append(cursor.axisX.min, newCursorPosition);

    if (newCursorPosition !== chartView.yCursorData[cursorIndex]) {
        chartView.yCursorData[cursorIndex] = newCursorPosition;
        chartView.cursorDataChanged = true;
    }

    chartView.yCursorPrevPosition[cursorIndex] = newPosition;
    cursor.visible = true;
}

function cursorDisplayChanged(chartView, isOn) {
    for (var i = 0; i < chartView.xCursorList.length; ++i) {
        chartView.xCursorList[i].visible = isOn
    }
    for (var j = 0; j < chartView.yCursorList.length; ++j) {
        chartView.yCursorList[j].visible = isOn
    }
}
