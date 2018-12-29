#ifndef LOGGERMODULE_H
#define LOGGERMODULE_H

#include "QTextStream"
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QPointF>
#include <QQmlApplicationEngine>
#include <QReadLocker>
#include <QReadWriteLock>
#include <QVariant>

class LoggerModule : public QObject {
  Q_OBJECT
public:
  explicit LoggerModule(QList<QList<QVector<QPointF> *>> &newDataBuffer,
                        QList<QReadWriteLock *> newDataLock);
  static QObject *singletonProvider(QQmlEngine *engine,
                                    QJSEngine *scriptEngine);

signals:
  void totalPointChanged(QVariant newTotalPoint);

public slots:
  QString switchLogger(const bool isRecording, const QVariant enabledList,
                       const QString destUrl, const QVariant nameList);
  void setGuiSource(QObject *measureGUI);
  void updateModule(int currBufferIndex);

private:
  static LoggerModule *singleton;

  QList<QList<QVector<QPointF> *>> &newDataBuffer;
  QList<QReadWriteLock *> newDataLock_;

  bool isLogging = false;
  int totalPoint = 0;

  QFile *dataFile = nullptr;
  QTextStream *dataStream = nullptr;
  QMutex enabledListMutex;
  QList<int> enabledIndexList;
};

#endif // LOGGERMODULE_H
