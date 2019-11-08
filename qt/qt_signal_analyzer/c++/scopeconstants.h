#ifndef SCOPECONSTANTS_H
#define SCOPECONSTANTS_H

#include <QtCore>

static constexpr int kTotalSeries = 4;
static constexpr int kMaxTotalPoints = 100000;
static constexpr int kNewPointsPerTransfer = 1000;
static constexpr qreal kStorageThreshold = 0.90;
static constexpr qreal kTrimPercentage = 0.5;
static constexpr qreal kXAxisUpateOffset = 5;

static constexpr int kTotalNewDataBuffer = 2;

static constexpr qreal kAxisYMax = 5;
static constexpr qreal kAxisYMin = -5;

#endif // SCOPECONSTANTS_H
