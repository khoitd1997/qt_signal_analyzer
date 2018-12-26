#ifndef MATHMODULE_H
#define MATHMODULE_H

#include "QList"
#include "QObject"
#include "QQmlListProperty"
#include <QPointF>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QXYSeries>
#include <algorithm>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MeasureModule : public QObject {
  Q_OBJECT

public:
  explicit MeasureModule(QList<QList<QPointF> *> &dataList);
  ~MeasureModule();

signals:
  void measureFinished();

public:
  enum MeasureType { Freq = 0, Period, Max, Min, PeakToPeak };
  Q_ENUM(MeasureType)

public slots:
  void setGuiSource(QObject *measureGUI);
  void updateMeasure(void);
  QList<qreal> getResult(void);

private:
  qreal getMaxY(const int sourceIndex);
  qreal getMinY(const int sourceIndex);
  qreal getPeriod(const int sourceIndex);
  qreal getPeakToPeak(const int sourceIndex);

  QList<QPointF>::reverse_iterator
  findFirstPointInRange(QList<QPointF>::reverse_iterator beginPoint,
                        QList<QPointF>::reverse_iterator endPoint, qreal &range,
                        const qreal rangeIncrement,
                        std::function<bool(const QPointF &)> comp);

  QList<QList<QPointF> *> &allData;
  QList<qreal> measResult;
  QObject *measureGUI;
};

#endif // MATHMODULE_H
