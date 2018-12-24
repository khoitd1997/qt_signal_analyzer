#ifndef DATAWORKER_H
#define DATAWORKER_H

#include <QList>
#include <QMutex>
#include <QObject>
#include <QReadWriteLock>

static const int maxPoints = 100000;
static const double storageThreshold = 0.8;

class DataWorker : public QObject
{
  Q_OBJECT
public:
  explicit DataWorker(QReadWriteLock& dataLock, QList<QList<QPointF>*> rawData);

signals:
  void newDataReady();

public slots:
  void startWork(void);
  void update(void);

private:
  QReadWriteLock& dataLock;
  QList<QList<QPointF>*> rawData;
};

#endif // DATAWORKER_H
