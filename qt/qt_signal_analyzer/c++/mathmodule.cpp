#include <QDebug>
#include <QtMath>

#include "mathmodule.h"
#include "scopeconstants.h"

const QList<QString> MathModule::kMathNames({"A-B", "A+B", "Abs(A)", "Log(A)"});
MathModule *MathModule::singleton = nullptr;

MathModule::MathModule(QList<QVector<QPointF> *> &allData,
                       QReadWriteLock *allDataLock)
    : allDataLock_(allDataLock), allData_(allData) {
  MathModule::singleton = this;
}

QObject *MathModule::singletonProvider(QQmlEngine *engine,
                                       QJSEngine *scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  return MathModule::singleton;
}

void MathModule::setGuiSource(QObject *mathGui) {
  guiObj_ = mathGui;
  connect(mathGui, SIGNAL(mathChanged(int, int, int, int, bool)), this,
          SLOT(switchMath(int, int, int, int, bool)));
}

void MathModule::updateModule() {
  for (auto serieIndex = 0; serieIndex < series_.length(); ++serieIndex) {
    QMutexLocker lock(&mathDataMutex_);
    if (enabledList_[serieIndex]) {
      series_[serieIndex]->setVisible(true);
      QReadLocker lock(allDataLock_);
      auto sourceA = allData_[sourceAList_[serieIndex]];
      auto sourceB = allData_[sourceBList_[serieIndex]];
      qreal tempVar = 0;

      for (auto i = 0; i < sourceA->size(); ++i) {
        switch (typeList_[serieIndex]) {
        case Subtraction:
          tempVar = sourceA->operator[](i).y() - sourceB->operator[](i).y();
          break;
        case Addition:
          tempVar = sourceA->operator[](i).y() + sourceB->operator[](i).y();
          break;
        case Absolute:
          tempVar = qFabs(sourceA->operator[](i).y());
          break;
        case Logarithm:
          tempVar = qLn(sourceA->operator[](i).y());
          break;
        }
        mathSerieBuffers_[serieIndex]->append(
            QPointF(sourceA->operator[](i).x(), tempVar));
      }
      series_[serieIndex]->replace(*mathSerieBuffers_[serieIndex]);
      mathSerieBuffers_[serieIndex]->clear();
    } else {
      series_[serieIndex]->setVisible(false);
    }
  }
  //   qDebug() << "Math: done updating serie" << endl;
}

void MathModule::switchMath(const int slotIndex, const int newType,
                            const int newASource, const int newBSource,
                            const bool isEnabled) {
  //   qDebug() << "Math module notified of switch" << endl;
  QMutexLocker lock(&mathDataMutex_);
  auto newMathType = static_cast<MathType>(newType);

  if (newMathType != typeList_[slotIndex]) {
    serieChangedType_[slotIndex] = true;
    typeList_[slotIndex] = newMathType;
  }
  sourceAList_[slotIndex] = newASource;
  sourceBList_[slotIndex] = newBSource;
  enabledList_[slotIndex] = isEnabled;
  series_[slotIndex]->setVisible(isEnabled);
}

void MathModule::addSeries(QVariantList series) {
  QMutexLocker lock(&mathDataMutex_);
  for (auto i = 0; i < series.size(); ++i) {
    // qDebug() << "Adding math series" << endl;
    series_.append(qvariant_cast<QXYSeries *>(series[i]));
    typeList_.append(static_cast<MathType>(0));
    sourceAList_.append(0);
    sourceBList_.append(0);
    enabledList_.append(false);
    serieChangedType_.append(false);
    mathSerieBuffers_.append(new QVector<QPointF>());
    mathSerieBuffers_[i]->reserve(kMaxTotalPoints);
  }
}
