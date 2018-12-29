#ifndef MEASUREMODULE_H
#define MEASUREMODULE_H

#include "QList"
#include "QObject"
#include <QMutex>
#include <QMutexLocker>
#include <QPointF>
#include <QQmlApplicationEngine>
#include <QReadLocker>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QXYSeries>
#include <algorithm>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MeasureModule : public QObject
{
  Q_OBJECT

public:
  MeasureModule(QList<QList<QPointF>*>& allData, QReadWriteLock* allDataLock);
  static QObject* singletonProvider(QQmlEngine* engine,
                                    QJSEngine* scriptEngine);

  ~MeasureModule();

signals:
  void measureFinished();

public:
  enum MeasureType
  {
    Freq = 0,
    Period,
    Max,
    Min,
    PeakToPeak
  };
  Q_ENUM(MeasureType)

public slots:
  void addMeasurements(const int totalMeasurements);
  void setGuiSource(QObject* measureGUI);
  void switchMeasure(const int slotIndex,
                     const int newType,
                     const int newSource,
                     const bool isEnabled);
  void updateModule(void);
  QList<qreal> getResult(void);

private:
  QList<QPointF>::reverse_iterator findFirstPointInRange(
    QList<QPointF>::reverse_iterator beginPoint,
    QList<QPointF>::reverse_iterator endPoint,
    qreal& range,
    const qreal rangeIncrement,
    std::function<bool(const QPointF&)> comp);

  qreal getMaxY(const int sourceIndex);
  qreal getMinY(const int sourceIndex);
  qreal getPeriod(const int sourceIndex);
  qreal getPeakToPeak(const int sourceIndex);

  static MeasureModule* singleton;

  QReadWriteLock* allDataLock_;
  QList<QList<QPointF>*>& allData_;

  QMutex resultMutex;
  QList<qreal> measResult;

  QMutex measureDataMutex;
  QList<int> typeList;
  QList<int> sourceList;
  QList<bool> enabledList;
};

#endif // MEASUREMODULE_H
