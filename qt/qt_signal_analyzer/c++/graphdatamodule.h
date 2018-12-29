#ifndef GRAPHDATAMODULE_H
#define GRAPHDATAMODULE_H

#include <QAbstractAxis>
#include <QMutex>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QReadLocker>
#include <QXYSeries>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class GraphDataModule : public QObject {
  Q_OBJECT
public:
  GraphDataModule(QList<QVector<QPointF> *> &allData,
                  QReadWriteLock *allDataLock);
  static QObject *singletonProvider(QQmlEngine *engine,
                                    QJSEngine *scriptEngine);

signals:
  void graphDataFinished();

public slots:
  void setGuiSource(QObject *graphGUI);
  void updateModule(void);

  void changeSerieName(const int serieIndex, const QString newName);
  void changeSerieDisplay(const int serieIndex, const bool isOn);
  void addSeries(QVariantList series);

private:
  static GraphDataModule *singleton;

  QMutex serieMutex_;
  QReadWriteLock *allDataLock_;
  QList<QXYSeries *> series_;
  QObject *guiObj_;
  QList<QVector<QPointF> *> &allData_;
};

#endif // GRAPHDATAMODULE_H
