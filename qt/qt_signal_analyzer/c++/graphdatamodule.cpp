#include "graphdatamodule.h"

#include <QtDebug>

GraphDataModule::GraphDataModule(const QList<QList<QPointF> *> &dataList)
    : m_data(dataList), serieList(QList<QXYSeries *>()) {}

void GraphDataModule::updateLineSeries() {
  for (auto i = 0; i < serieList.length(); ++i) {
    this->serieList[i]->replace(*m_data[i]);
  }
  //   qDebug() << "Done Updating line series from thread" << endl;
  emit graphDataFinished();
}

void GraphDataModule::updateSeriesRef(QVariantList rawSerie) {
  foreach (auto serie, rawSerie) {
    this->serieList.append(qvariant_cast<QXYSeries *>(serie));
  }
}
