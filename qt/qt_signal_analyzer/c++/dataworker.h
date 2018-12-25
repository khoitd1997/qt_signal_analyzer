#ifndef DATAWORKER_H
#define DATAWORKER_H

#include <QList>
#include <QMutex>
#include <QObject>
#include <QReadWriteLock>

static const int NEW_POINTS_PER_EVENT = 1000;

class DataWorker : public QObject
{
  Q_OBJECT
public:
  explicit DataWorker(QList<QList<QList<QPointF>*>>& newDataBuffer);

signals:
  void newDataReady();

public slots:
  void startWork(void);
  void update(void);

private:
  const int totalBuffer;
  int currBufferIndex = 0;

  QList<QList<QList<QPointF>*>>& newDataBuffer;
};

#endif // DATAWORKER_H
