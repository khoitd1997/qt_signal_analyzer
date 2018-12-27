#ifndef SCOPESERIEMODULE_H
#define SCOPESERIEMODULE_H

#include <QObject>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QXYSeries>

#include "scopemodule.h"

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class ScopeSerieModule : public ScopeModule {
public:
  ScopeSerieModule() = delete;
  ScopeSerieModule(QList<QList<QList<QPointF> *>> &newDataBuffer,
                   QList<QList<QPointF> *> &allData);
  virtual ~ScopeSerieModule();

public slots:
  virtual void addSerie(QAbstractSeries *serie);
  virtual void addSerie(QVariantList series);
  virtual void addSerie(QXYSeries *serie);
  virtual void updateModule(int currBufIndex) = 0;
  bool needTrim(const int totalPoints);

protected:
  QList<QXYSeries *> series_;
  QList<QList<QPointF> *> &allData_;
};

#endif // SCOPESERIEMODULE_H
