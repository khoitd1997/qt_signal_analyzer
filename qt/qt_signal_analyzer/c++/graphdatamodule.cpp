#include "graphdatamodule.h"
#include "scopeconstants.h"

#include <QValueAxis>
#include <QtDebug>

GraphDataModule* GraphDataModule::singleton = nullptr;

GraphDataModule::GraphDataModule(QList<QList<QPointF>*>& allData,
                                 QReadWriteLock* allDataLock)
  : allDataLock_(allDataLock)
  , allData_(allData)
{
  GraphDataModule::singleton = this;
}

QObject*
GraphDataModule::singletonProvider(QQmlEngine* engine, QJSEngine* scriptEngine)
{
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  return GraphDataModule::singleton;
}

void
GraphDataModule::setGuiSource(QObject* graphGUI)
{
  guiObj_ = graphGUI;
  connect(this, SIGNAL(graphDataFinished()), guiObj_, SLOT(updateGraphs()));
}

void
GraphDataModule::addSerie(QVariantList series)
{
  foreach (auto s, series) {
    series_.append(qvariant_cast<QXYSeries*>(s));
  }
}

void
GraphDataModule::changeSerieName(const int serieIndex, const QString newName)
{
  QMutexLocker lock(&serieMutex_);
  series_[serieIndex]->setName(newName);
}

void
GraphDataModule::changeSerieDisplay(const int serieIndex, const bool isOn)
{
  QMutexLocker lock(&serieMutex_);
  series_[serieIndex]->setVisible(isOn);
}

void
GraphDataModule::updateModule(void)
{
  QReadLocker lock(allDataLock_);

  for (auto serieIndex = 0; serieIndex < series_.size(); ++serieIndex) {
    auto currSerie = series_[serieIndex];
    currSerie->replace(*allData_[serieIndex]);
  }

  emit graphDataFinished();
}
