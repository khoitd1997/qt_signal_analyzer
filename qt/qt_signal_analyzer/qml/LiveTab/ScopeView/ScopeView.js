function updateGraph(chartViewObj, serieIndex, xAxis) {
    if (chartViewObj.serieList[serieIndex].visible) {
        var serie = chartViewObj.serieList[serieIndex]
//        var xAxis = chartViewObj.xAxis

        xAxis.min += serie.at(serie.count - 1).x + 5 - xAxis.max;
        xAxis.max += serie.at(serie.count - 1).x + 5 - xAxis.max;
    }

    // for (var i = 0; i < chartViewObj.xCursorList.length; ++i) {
    //     updateXCursor(i)
    // }
    // for (var j = 0; j < chartViewObj.yCursorList.length; ++j) {
    //     updateYCursor(j)
    // }

    // if (cursorDataChanged) {
    //     newCursorData(chartViewObj.xCursorData[1] - chartViewObj.xCursorData[0],
    //         chartViewObj.yCursorData[0], chartViewObj.yCursorData[1],
    //         chartViewObj.yCursorData[1] - chartViewObj.yCursorData[0]);
    // }

    // chartViewObj.cursorDataChanged = false
}

// function test(chartViewObj) {
//     console.log("js test:" + chartViewObj.openGL);
// }
