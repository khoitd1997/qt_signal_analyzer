#ifndef DATAWORKER_H
#define DATAWORKER_H

#include <QList>
#include <QMutex>
#include <QObject>
#include <QReadWriteLock>
#include <QString>
#include <QTimer>

#include <memory>

#include "scopeconstants.h"
#include "signalanalyzerdevice.hpp"

class DataWorker : public QObject {
  Q_OBJECT
 public:
  explicit DataWorker(QList<QList<QVector<QPointF> *>> &newDataBuffer,
                      QList<QReadWriteLock *>           newDataLock);

 signals:
  void newDataReady(int curBufIndex);

 public slots:
  void startWork();
  void processData();
  void changeSignalSource(QString newSource);

 private:
  void incrementBufIndex();
  void prepareSignalSource();

  QTimer timer_;

  QString currSignalSource_    = "Simulation";
//  QString currSignalSource_    = "ttyACM0";
  std::shared_ptr<SignalAnalyzerDevice> device_ = nullptr;

  int                               curBufIndex_ = 0;
  QList<QReadWriteLock *>           newDataLock_;
  QList<QList<QVector<QPointF> *>> &newDataBuffer_;
};

#endif  // DATAWORKER_H
