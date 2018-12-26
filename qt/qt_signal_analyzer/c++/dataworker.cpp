#include "dataworker.h"

#include "QTimer"
#include "QtDebug"
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>

static const uint pointsPerInc = 500;

DataWorker::DataWorker(QList<QList<QList<QPointF> *>> &newDataBuffer)
    : totalBuffer(newDataBuffer.size()), newDataBuffer(newDataBuffer) {}

void DataWorker::startWork(void) {
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(300);
}

void DataWorker::update(void) {
  static unsigned long currLimit = 0;
  currLimit += pointsPerInc;
  auto serieIndex = 0;

  foreach (auto dataSerie, newDataBuffer[currBufferIndex]) {
    for (auto j(currLimit); j < currLimit + pointsPerInc; j++) {
      qreal x(0);
      qreal y(0);
      y = qSin(M_PI / 6000 * j) + 1.5;
      //   + 0.5 * (serieIndex + 1);
      x = j;
      dataSerie->append(QPointF(x, y));
    }
    // qDebug() << "Data Worker: Generating serie: " << serieIndex << endl;
    ++serieIndex;
  }
  emit newDataReady();

  currBufferIndex =
      (currBufferIndex == totalBuffer - 1) ? 0 : currBufferIndex + 1;
  //   qDebug() << "Data Worker: Current buffer index: " << currBufferIndex <<
  //   endl;
}
