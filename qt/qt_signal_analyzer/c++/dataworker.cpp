#include "dataworker.h"

#include <QThread>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>
#include "QTimer"
#include "QtDebug"

#include "channel_data.h"
#include "signalanalyzerdevice.hpp"

DataWorker::DataWorker(QList<QList<QVector<QPointF> *>> &newDataBuffer,
                       QList<QReadWriteLock *>           newDataLock)
    : newDataLock_(newDataLock), newDataBuffer_(newDataBuffer) {}

void DataWorker::startWork(void) {
  while (1) {
    static int currLimit = 0;
    currLimit += kNewPointsPerTransfer;

    while (!(newDataLock_[curBufIndex_]->tryLockForWrite(0))) { incrementBufIndex(); }
    if (currSignalSource_.isEmpty() || currSignalSource_ == "Simulation") {
      auto serieIndex = 0;

      for (auto dataSerie : newDataBuffer_[curBufIndex_]) {
        for (auto j(currLimit); j < currLimit + kNewPointsPerTransfer; j++) {
          qreal      y                         = qSin(M_PI / (50 * (serieIndex + 1)) * j) + 1.5;
          qreal      x                         = j;
          dataSerie->operator[](j - currLimit) = QPointF(x, y);
        }
        ++serieIndex;
      }

      newDataLock_[curBufIndex_]->unlock();
      emit newDataReady(curBufIndex_);
      incrementBufIndex();

      QThread::msleep(400);
    }
  }
}

void DataWorker::incrementBufIndex(void) {
  curBufIndex_ = (curBufIndex_ + 1) % newDataBuffer_.size();
}
