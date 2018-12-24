#ifndef MATHMODULE_H
#define MATHMODULE_H

#include "QList"
#include "QObject"
#include "QQmlListProperty"
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QXYSeries>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MeasureModule : public QObject
{
  Q_OBJECT

public:
  explicit MeasureModule(QList<QList<QPointF>*>& dataList);
  ~MeasureModule();

signals:
  void measureFinished();

public slots:
  void setGuiSource(QObject* measureGUI);
  void updateMeasure(void);
  QList<qreal> getResult(void);

private:
  QList<QList<QPointF>*>& m_data;
  QList<qreal> measResult;
  QObject* measureGUI;
};

#endif // MATHMODULE_H
