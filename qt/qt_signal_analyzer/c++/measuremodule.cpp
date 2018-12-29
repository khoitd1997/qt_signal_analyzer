#include "measuremodule.h"

#include "QObject"
#include "QTimer"
#include "QtDebug"
#include <QQmlProperty>

static const qreal kDefaultMidPointRange = 0.03;
static const qreal kMidPointRangeInc = 0.02;
static const int kMaxPeriodCalcTrial = 7;

MeasureModule *MeasureModule::singleton = nullptr;

MeasureModule::MeasureModule(QList<QVector<QPointF> *> &allData_,
                             QReadWriteLock *allDataLock)
    : allDataLock_(allDataLock), allData_(allData_) {
  singleton = this;
}

QObject *MeasureModule::singletonProvider(QQmlEngine *engine,
                                          QJSEngine *scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  return MeasureModule::singleton;
}

MeasureModule::~MeasureModule() {}

void MeasureModule::setGuiSource(QObject *measureGUI) {
  connect(measureGUI, SIGNAL(measureChanged(int, int, int, bool)), this,
          SLOT(switchMeasure(int, int, int, bool)));
  connect(this, SIGNAL(measureFinished()), measureGUI,
          SLOT(changeMeasureResults()));
}

void MeasureModule::addMeasurements(const int totalMeasurements) {
  for (auto i = 0; i < totalMeasurements; ++i) {
    typeList.append(0);
    sourceList.append(0);
    enabledList.append(false);
    measResult.append(0);
  }
}

QList<qreal> MeasureModule::getResult(void) {
  QMutexLocker resultLock(&resultMutex);
  return measResult;
}

qreal MeasureModule::getMaxY(const int sourceIndex) {
  return (std::max_element(allData_[sourceIndex]->begin(),
                           allData_[sourceIndex]->end(),
                           [](const QPointF &a, const QPointF &b) {
                             return (a.y() < b.y());
                           }))
      ->y();
}
qreal MeasureModule::getMinY(const int sourceIndex) {
  return (std::min_element(allData_[sourceIndex]->begin(),
                           allData_[sourceIndex]->end(),
                           [](const QPointF &a, const QPointF &b) {
                             return (a.y() < b.y());
                           }))
      ->y();
}

qreal MeasureModule::getPeakToPeak(const int sourceIndex) {
  return getMaxY(sourceIndex) - getMinY(sourceIndex);
}

QVector<QPointF>::reverse_iterator MeasureModule::findFirstPointInRange(
    QVector<QPointF>::reverse_iterator beginPoint,
    QVector<QPointF>::reverse_iterator endPoint, qreal &range,
    const qreal rangeIncrement, std::function<bool(const QPointF &)> comp) {
  auto res = endPoint;
  auto trialCount = 0;

  do {
    res = std::find_if(beginPoint, endPoint, comp);

    if (trialCount > kMaxPeriodCalcTrial) {
      return res;
    }
    range += rangeIncrement;
    ++trialCount;
  } while (res == endPoint);
  return res;
}

qreal MeasureModule::getPeriod(const int sourceIndex) {
  qreal range = kDefaultMidPointRange;
  qreal midPointY = getMinY(sourceIndex) + getPeakToPeak(sourceIndex) / 2.0;
  auto maxY = getMaxY(sourceIndex);

  auto startPoint = findFirstPointInRange(
      allData_[sourceIndex]->rbegin(), allData_[sourceIndex]->rend(), range,
      kMidPointRangeInc, [maxY, range](const QPointF &p) {
        return ((p.y() < maxY + range) && (p.y() > maxY - range));
      });
  if (allData_[sourceIndex]->rend() == startPoint) {
    return -1;
  }
  range = kDefaultMidPointRange;

  auto midPoint1 = findFirstPointInRange(
      startPoint, allData_[sourceIndex]->rend(), range, kMidPointRangeInc,
      [midPointY, range](const QPointF &p) {
        return (p.y() < midPointY + range) && (p.y() > midPointY - range);
      });
  if (allData_[sourceIndex]->rend() == midPoint1) {
    return -1;
  }
  range = kDefaultMidPointRange;

  auto endMidPoint1 = std::find_if(midPoint1, allData_[sourceIndex]->rend(),
                                   [midPointY, range](const QPointF &p) {
                                     return (p.y() < midPointY - range);
                                   });
  if (endMidPoint1 == allData_[sourceIndex]->rend()) {
    return -1;
  }
  range = kDefaultMidPointRange;

  auto midPoint2 = findFirstPointInRange(
      endMidPoint1 + 1, allData_[sourceIndex]->rend(), range, kMidPointRangeInc,
      [midPointY, range](const QPointF &p) {
        return (p.y() < midPointY + range) && (p.y() > midPointY - range);
      });
  if (allData_[sourceIndex]->rend() == midPoint2) {
    return -1;
  }

  return (midPoint1->x() - midPoint2->x()) * 2;
}

void MeasureModule::switchMeasure(const int slotIndex, const int newType,
                                  const int newSource, const bool isEnabled) {
  QMutexLocker lock(&measureDataMutex);
  typeList[slotIndex] = newType;
  sourceList[slotIndex] = newSource;
  enabledList[slotIndex] = isEnabled;
}

void MeasureModule::updateModule() {
  QMutexLocker dataLock(&measureDataMutex);
  QMutexLocker resultLock(&resultMutex);
  int sourceIndex = 0;

  for (auto slotIndex = 0; slotIndex < enabledList.size(); ++slotIndex) {
    if (enabledList[slotIndex]) {
      sourceIndex = sourceList[slotIndex];
      qreal temp = 0.0;
      switch (static_cast<MeasureType>(typeList[slotIndex])) {
      case MeasureType::Max:
        temp = getMaxY(sourceIndex);
        break;
      case MeasureType::Min:
        temp = getMinY(sourceIndex);
        break;
      case MeasureType::Freq:
        temp = 1.0 / getPeriod(sourceIndex);
        break;
      case MeasureType::Period:
        temp = getPeriod(sourceIndex);
        break;
      case MeasureType::PeakToPeak:
        temp = getPeakToPeak(sourceIndex);
        break;
      }
      measResult[slotIndex] = temp;
    }
  }

  qDebug() << "emitted measure finished" << endl;
  emit measureFinished();
}
