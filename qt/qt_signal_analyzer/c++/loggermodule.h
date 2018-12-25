#ifndef LOGGERMODULE_H
#define LOGGERMODULE_H

#include "QTextStream"
#include <QFile>
#include <QObject>
#include <QPointF>
#include <QVariant>

class LoggerModule : public QObject {
  Q_OBJECT
public:
  explicit LoggerModule(QList<QList<QList<QPointF> *>> &newDataBuffer);

signals:
  void loggerFinished();

public slots:
  QString switchLogger(const bool isRecording, const QVariant enabledList,
                       const QString destUrl, const QVariant nameList);
  void setGuiSource(QObject *measureGUI);
  void updateLogger(int currBufferIndex);

private:
  QList<QList<QList<QPointF> *>> &newDataBuffer;

  bool isLogging = false;
  uint totalPoint = 0;

  QFile *dataFile = nullptr;
  QTextStream *dataStream = nullptr;
  QList<int> enabledIndexList;

  QObject *guiObj = nullptr;
};

#endif // LOGGERMODULE_H
