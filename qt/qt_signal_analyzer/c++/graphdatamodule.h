#ifndef GRAPHDATAMODULE_H
#define GRAPHDATAMODULE_H

#include <QMutex>
#include <QObject>
#include <QReadLocker>
#include <QReadWriteLock>
#include <QWaitCondition>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QXYSeries>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class GraphDataModule : public QObject
{
  Q_OBJECT
public:
  explicit GraphDataModule(const QList<QList<QPointF>*>& dataList);
signals:
  void graphDataFinished();

public slots:
  void updateSeriesRef(QVariantList rawSerie);
  void updateLineSeries();

private:
  const QList<QList<QPointF>*>& m_data;
  QList<QXYSeries*> serieList;
};

#endif // GRAPHDATAMODULE_H
