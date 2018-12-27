#include "scopeseriemodule.h"
#include "scopeconstants.h"

ScopeSerieModule::ScopeSerieModule(
    QList<QList<QList<QPointF> *>> &newDataBuffer,
    QList<QList<QPointF> *> &allData)
    : ScopeModule(newDataBuffer), allData_(allData) {}

ScopeSerieModule ::~ScopeSerieModule() {}

void ScopeSerieModule::addSerie(QAbstractSeries *serie) {
  series_.append(static_cast<QXYSeries *>(serie));
}

void ScopeSerieModule::addSerie(QXYSeries *serie) { series_.append(serie); }

void ScopeSerieModule::addSerie(QVariantList series) {
  foreach (auto s, series) { addSerie(qvariant_cast<QXYSeries *>(s)); }
}

bool ScopeSerieModule::needTrim(const int totalPoints) {
  return totalPoints > (kStorageThreshold * kMaxTotalPoints);
}