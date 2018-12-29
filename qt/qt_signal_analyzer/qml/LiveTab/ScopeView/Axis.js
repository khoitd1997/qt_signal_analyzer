function changeAxisOffset(chartView, xAxis, id, axisName, newOffset) {
    if (axisName === "x") {
        xAxis.min += (newOffset - oldXOffset);
        xAxis.max += (newOffset - oldXOffset);
        oldXOffset = newOffset;
    } else if (axisName === "y") {
        if (id % 2 != 0) {
            chartView.serieList[id].axisYRight.max += (newOffset - oldYOffset);
            chartView.serieList[id].axisYRight.min += (newOffset - oldYOffset);
        } else {
            chartView.serieList[id].axisY.max += (newOffset - oldYOffset);
            chartView.serieList[id].axisY.min += (newOffset - oldYOffset);
        }
        chartView.oldYOffset = newOffset;
    }
}

function scrollXAxis(chartView, xAxis) {
    var serie = chartView.serieList[0];
    xAxis.min += serie.at(serie.count - 1).x + 5 - xAxis.max;
    xAxis.max += serie.at(serie.count - 1).x + 5 - xAxis.max;
}

function changeAxisRange(chartView, xAxis, id, axisName, newRange) {
    var serie = chartView.serieList[id];
    if (axisName === "x") {
        xAxis.max += (newRange - (serie.axisX.max - serie.axisX.min)) / 2;
        xAxis.min -= (newRange - (serie.axisX.max - serie.axisX.min)) / 2;

    } else if (axisName === "y") {
        if ((id % 2) != 0) {
            serie.axisYRight.max += (newRange - (serie.axisYRight.max - serie.axisYRight.min)) / 2.0;
            serie.axisYRight.min -= (newRange - (serie.axisYRight.max - serie.axisYRight.min)) / 2.0;
        } else {
            serie.axisY.max += (newRange - (serie.axisY.max - serie.axisY.min)) / 2.0;
            serie.axisY.min -= (newRange - (serie.axisY.max - serie.axisY.min)) / 2.0;
        }
    }
}