#ifndef MATHMODULE_H
#define MATHMODULE_H

#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QReadLocker>
#include <QReadWriteLock>
#include <QVariant>
#include <QXYSeries>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MathModule : public QObject {
  Q_OBJECT
public:
  MathModule(QList<QVector<QPointF> *> &allData, QReadWriteLock *allDataLock);
  enum MathType { Subtraction = 0, Addition, Absolute, Logarithm };
  Q_ENUM(MathType)

  static QObject *singletonProvider(QQmlEngine *engine,
                                    QJSEngine *scriptEngine);

signals:

public slots:
  void setGuiSource(QObject *mathGUI);
  void addSeries(QVariantList series);
  void switchMath(const int slotIndex, const int newType, const int newASource,
                  const int newBSource, const bool isEnabled);
  void updateModule();

private:
  static MathModule *singleton;

  QReadWriteLock *allDataLock_;
  QList<QVector<QPointF> *> &allData_;

  QMutex mathDataMutex_;
  QList<MathType> typeList_;
  QList<int> sourceAList_;
  QList<int> sourceBList_;
  QList<bool> enabledList_;
  QList<bool> serieChangedType_;

  QList<QVector<QPointF> *> mathSerieBuffers_;

  static const QList<QString> kMathNames;
  QList<QXYSeries *> series_;

  QObject *guiObj_;
};

#endif // MATHMODULE_H
