#ifndef DATAWORKER_H
#define DATAWORKER_H

#include <QList>
#include <QMutex>
#include <QObject>
#include <QReadWriteLock>

#include "scopeconstants.h"

class DataWorker : public QObject {
  Q_OBJECT
public:
  explicit DataWorker(QList<QList<QList<QPointF> *>> &newDataBuffer,
                      QList<QReadWriteLock *> newDataLock);

signals:
  void newDataReady(int curBufIndex);

public slots:
  void startWork(void);
  void update(void);

private:
  void incrementBufIndex(void);

  const int totalBuffer;
  int curBufIndex_ = 0;

  QList<QReadWriteLock *> newDataLock_;
  QList<QList<QList<QPointF> *>> &newDataBuffer_;
};

#endif // DATAWORKER_H
