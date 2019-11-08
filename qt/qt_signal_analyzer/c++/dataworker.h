#ifndef DATAWORKER_H
#define DATAWORKER_H

#include <QList>
#include <QMutex>
#include <QObject>
#include <QReadWriteLock>
#include <QString>

#include "scopeconstants.h"

class DataWorker : public QObject {
  Q_OBJECT
 public:
  explicit DataWorker(QList<QList<QVector<QPointF> *>> &newDataBuffer,
                      QList<QReadWriteLock *>           newDataLock);

 signals:
  void newDataReady(int curBufIndex);

 public slots:
  void startWork(void);

 private:
  void incrementBufIndex(void);

  QString currDevicePath_ = {};

  int                               curBufIndex_ = 0;
  QList<QReadWriteLock *>           newDataLock_;
  QList<QList<QVector<QPointF> *>> &newDataBuffer_;
};

#endif  // DATAWORKER_H
