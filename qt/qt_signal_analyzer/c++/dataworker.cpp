#include "dataworker.h"

#include "QTimer"
#include "QtDebug"
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>

DataWorker::DataWorker(QReadWriteLock& dataLock, QList<QList<QPointF>*> rawData)
  : dataLock(dataLock)
  , rawData(rawData)
{}

void
DataWorker::startWork(void)
{
  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(300);
}

void
DataWorker::update(void)
{
  QWriteLocker locker(&dataLock);
  static unsigned long currLimit = 0;
  currLimit += 1024;
  auto serieIndex = 0;

  foreach (auto dataSerie, rawData) {
    if (dataSerie->size() > storageThreshold * maxPoints) {
      qDebug() << "Free elements" << endl;
      dataSerie->erase(dataSerie->begin(),
                       dataSerie->begin() + dataSerie->size() / 2);
    }

    for (auto j(currLimit); j < currLimit + 1024; j++) {
      qreal x(0);
      qreal y(0);
      y = qSin(M_PI / 50 * j) + 0.5 * (serieIndex + 1) +
          QRandomGenerator::global()->generateDouble();
      x = j;
      dataSerie->append(QPointF(x, y));
      //    qDebug() << "Generating x: " << x << endl;
    }
    ++serieIndex;
  }
  emit newDataReady();
}
