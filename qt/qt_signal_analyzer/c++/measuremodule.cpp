#include "measuremodule.h"

#include "QObject"
#include "QTimer"
#include "QtDebug"
#include <QQmlListProperty>
#include <QQmlProperty>
// Q_DECLARE_METATYPE(MeasureModule)
// Q_DECLARE_METATYPE(MeasureModule*)

// using MeasureType = MeasureModule::MeasureType;
static const qreal DEFAULT_MIDPOINT_RANGE = 0.03;
static const qreal MIDPOINT_RANGE_INC = 0.02;
static const int MAX_PERIOD_CALC_TRIAL = 7;

MeasureModule::MeasureModule(QList<QList<QPointF> *> &dataList)
    : QObject(), allData(dataList) {
  measResult.append(0);
  measResult.append(0);
  measResult.append(0);
  measResult.append(0);
}

MeasureModule::~MeasureModule() {}

void MeasureModule::setGuiSource(QObject *measureGUI) {
  this->measureGUI = measureGUI;
}

QList<qreal> MeasureModule::getResult(void) { return measResult; }

qreal MeasureModule::getMaxY(const int sourceIndex) {
  return (std::max_element(allData[sourceIndex]->begin(),
                           allData[sourceIndex]->end(),
                           [](const QPointF &a, const QPointF &b) {
                             return (a.y() < b.y());
                           }))
      ->y();
}
qreal MeasureModule::getMinY(const int sourceIndex) {
  return (std::min_element(allData[sourceIndex]->begin(),
                           allData[sourceIndex]->end(),
                           [](const QPointF &a, const QPointF &b) {
                             return (a.y() < b.y());
                           }))
      ->y();
}

qreal MeasureModule::getPeakToPeak(const int sourceIndex) {
  return getMaxY(sourceIndex) - getMinY(sourceIndex);
}

QList<QPointF>::reverse_iterator MeasureModule::findFirstPointInRange(
    QList<QPointF>::reverse_iterator beginPoint,
    QList<QPointF>::reverse_iterator endPoint, qreal &range,
    const qreal rangeIncrement, std::function<bool(const QPointF &)> comp) {
  auto res = endPoint;
  auto trialCount = 0;

  do {
    res = std::find_if(beginPoint, endPoint, comp);

    if (trialCount > MAX_PERIOD_CALC_TRIAL) {
      return res;
    }
    range += rangeIncrement;
    ++trialCount;
  } while (res == endPoint);
  return res;
}

qreal MeasureModule::getPeriod(const int sourceIndex) {
  qreal range = DEFAULT_MIDPOINT_RANGE;
  qreal midPointY = getMinY(sourceIndex) + getPeakToPeak(sourceIndex) / 2.0;
  auto maxY = getMaxY(sourceIndex);

  auto startPoint = findFirstPointInRange(
      allData[sourceIndex]->rbegin(), allData[sourceIndex]->rend(), range,
      MIDPOINT_RANGE_INC, [maxY, range](const QPointF &p) {
        return ((p.y() < maxY + range) && (p.y() > maxY - range));
      });
  if (allData[sourceIndex]->rend() == startPoint) {
    return -1;
  }
  range = DEFAULT_MIDPOINT_RANGE;

  auto midPoint1 = findFirstPointInRange(
      startPoint, allData[sourceIndex]->rend(), range, MIDPOINT_RANGE_INC,
      [midPointY, range](const QPointF &p) {
        return (p.y() < midPointY + range) && (p.y() > midPointY - range);
      });
  if (allData[sourceIndex]->rend() == midPoint1) {
    return -1;
  }
  range = DEFAULT_MIDPOINT_RANGE;

  auto endMidPoint1 = std::find_if(midPoint1, allData[sourceIndex]->rend(),
                                   [midPointY, range](const QPointF &p) {
                                     return (p.y() < midPointY - range);
                                   });
  if (endMidPoint1 == allData[sourceIndex]->rend()) {
    return -1;
  }
  range = DEFAULT_MIDPOINT_RANGE;

  auto midPoint2 = findFirstPointInRange(
      endMidPoint1 + 1, allData[sourceIndex]->rend(), range, MIDPOINT_RANGE_INC,
      [midPointY, range](const QPointF &p) {
        return (p.y() < midPointY + range) && (p.y() > midPointY - range);
      });
  if (allData[sourceIndex]->rend() == midPoint2) {
    return -1;
  }

  return (midPoint1->x() - midPoint2->x()) * 2;
}

void MeasureModule::updateMeasure() {
  auto sourceList = (QQmlProperty::read(measureGUI, "sourceList")).toList();
  auto typeList = (QQmlProperty::read(measureGUI, "typeIndex")).toList();
  auto enabledList = (QQmlProperty::read(measureGUI, "enabledList")).toList();
  int sourceIndex = 0;

  for (auto slotIndex = 0; slotIndex < enabledList.size(); ++slotIndex) {
    if (enabledList[slotIndex].toBool()) {
      sourceIndex = sourceList[slotIndex].toInt();
      switch (static_cast<MeasureType>(typeList[slotIndex].toInt())) {
      case MeasureType::Max: // TODO: replace with enum
        measResult[slotIndex] = getMaxY(sourceIndex);
        break;
      case MeasureType::Min:
        measResult[slotIndex] = getMinY(sourceIndex);
        break;
      case MeasureType::Freq:
        measResult[slotIndex] = 1.0 / getPeriod(sourceIndex);
        break;
      case MeasureType::Period:
        measResult[slotIndex] = getPeriod(sourceIndex);
        break;
      case MeasureType::PeakToPeak:
        measResult[slotIndex] = getPeakToPeak(sourceIndex);
        break;
      }
    }
  }

  //   qDebug() << "Type Index result: " << endl;
  //   for (auto i = 0; i < variant.size(); ++i) {
  //     qDebug() << i << variant[i].toInt() << endl;
  //   }

  //   qDebug() << "Source List result: " << endl;
  //   for (auto i = 0; i < variant.size(); ++i) {
  //     qDebug() << i << variant[i].toInt() << endl;
  //   }

  emit measureFinished();
}
