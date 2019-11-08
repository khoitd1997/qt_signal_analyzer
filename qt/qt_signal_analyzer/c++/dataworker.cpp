#include "dataworker.h"

#include "QTimer"
#include "QtDebug"
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>

DataWorker::DataWorker(QList<QList<QVector<QPointF> *>> &newDataBuffer,
                       QList<QReadWriteLock *> newDataLock)
    : newDataLock_(newDataLock),
      newDataBuffer_(newDataBuffer) {}

void DataWorker::startWork(void) {
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(400);
}

void DataWorker::incrementBufIndex(void) {
  curBufIndex_ = (curBufIndex_ +1) % newDataBuffer_.size();
}

void DataWorker::update(void) {
  static int currLimit = 0;
  currLimit += kNewPointsPerTransfer;
  auto serieIndex = 1;

  while (!(newDataLock_[curBufIndex_]->tryLockForWrite(0))) {
    incrementBufIndex();
  }

  for (auto dataSerie: newDataBuffer_[curBufIndex_]) {
    for (auto j(currLimit); j < currLimit + kNewPointsPerTransfer; j++) {
      qreal y = qSin(M_PI / (50 * serieIndex) * j) + 1.5;
      qreal x = j;
      dataSerie->operator[](j - currLimit) = QPointF(x, y);
    }
    ++serieIndex;
  }

  newDataLock_[curBufIndex_]->unlock();
  emit newDataReady(curBufIndex_);
  incrementBufIndex();
}
