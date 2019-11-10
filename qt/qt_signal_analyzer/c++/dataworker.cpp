#include "dataworker.h"

#include <QThread>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>
#include "QtDebug"

#include "channel_data.h"

DataWorker::DataWorker(QList<QList<QVector<QPointF> *>> &newDataBuffer,
                       QList<QReadWriteLock *>           newDataLock)
    : timer_(this), newDataLock_(newDataLock), newDataBuffer_(newDataBuffer) {
    connect(&timer_, SIGNAL(timeout()), this, SLOT(processData()));
}

void DataWorker::startWork() {
   prepareSignalSource();
   timer_.start(50);
}

void DataWorker::processData(){
    ChannelDataPkt pkt;

    static int currLimit = 0;
    currLimit += kNewPointsPerTransfer;

    while (!(newDataLock_[curBufIndex_]->tryLockForWrite(0))) { incrementBufIndex(); }
    auto serieIndex = 0;
    if (currSignalSource_ == "Simulation") {
      for (auto dataSerie : newDataBuffer_[curBufIndex_]) {
        for (auto j(currLimit); j < currLimit + kNewPointsPerTransfer; j++) {
          qreal      y                         = qSin(M_PI / (50 * (serieIndex + 1)) * j) + 1.5;
          qreal      x                         = j;
          dataSerie->operator[](j - currLimit) = QPointF(x, y);
        }
        ++serieIndex;
      }
    } else {
        device_->getData(pkt);

        for (auto dataSerie : newDataBuffer_[curBufIndex_]) {
          for (auto j = 0; j < kMaxSamplePerPkt; j++) {
              if(serieIndex == 0){
                  qCritical() << pkt.adcData0[j] << " " << pkt.timestamp[j];
              }
            qreal      y                         = pkt.adcData0[j] * (3.3 / 4095);
            qreal      x                         = pkt.timestamp[j];
            dataSerie->operator[](j) = QPointF(x, y);
          }
          ++serieIndex;
        }
    }

      newDataLock_[curBufIndex_]->unlock();
      emit newDataReady(curBufIndex_);
      incrementBufIndex();
}

void DataWorker::changeSignalSource(QString newSource) {
  qCritical() << "New source: " << newSource;
  currSignalSource_    = newSource;
  prepareSignalSource();
}

void DataWorker::incrementBufIndex(void) {
  curBufIndex_ = (curBufIndex_ + 1) % newDataBuffer_.size();
}

void DataWorker::prepareSignalSource() {
    if(currSignalSource_ != "Simulation"){
        {
          // open device, do a dummy read, then close it
          ChannelDataPkt temp;
          SignalAnalyzerDevice("/dev/" + currSignalSource_.toStdString()).getData(temp);
        }
        device_ = std::make_shared<SignalAnalyzerDevice>("/dev/" + currSignalSource_.toStdString());
    }
}
