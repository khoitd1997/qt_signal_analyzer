#include <QDebug>

#include "loggermodule.h"

LoggerModule *LoggerModule::singleton = nullptr;

LoggerModule::LoggerModule(QList<QList<QList<QPointF> *>> &newDataBuffer,
                           QList<QReadWriteLock *> newDataLock)
    : newDataBuffer(newDataBuffer), newDataLock_(newDataLock) {
  LoggerModule::singleton = this;
}

QObject *LoggerModule::singletonProvider(QQmlEngine *engine,
                                         QJSEngine *scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  return LoggerModule::singleton;
}

QString LoggerModule::switchLogger(const bool isRecording,
                                   const QVariant enabledList,
                                   const QString destUrl,
                                   const QVariant nameList) {
  QMutexLocker enabledListLock(&enabledListMutex);

  if (isRecording) {
    auto tempEnabledList = enabledList.toList();
    for (auto i = 0; i < tempEnabledList.size(); ++i) {
      if (tempEnabledList[i].toBool()) {
        enabledIndexList.append(i);
      }
    }

    if (enabledIndexList.size() <= 0) {
      return "No line serie to record";
    }

    this->dataFile = new QFile(destUrl);
    if (this->dataFile->open(QIODevice::WriteOnly)) {
      this->dataStream = new QTextStream(this->dataFile);
      auto tempNameList = nameList.toList();
      *(this->dataStream) << "Time"
                          << "\t";

      foreach (auto serieIndex, enabledIndexList) {
        *(this->dataStream) << tempNameList[serieIndex].toString() << "\t";
      }
      isLogging = true;
    } else {
      return "Can't open csv file for write";
    }
  } else {
    isLogging = false;
    this->dataFile->close();
    this->totalPoint = 0;
    delete this->dataStream;
    delete this->dataFile;
    enabledIndexList.clear();
  }
  return "";
}

void LoggerModule::setGuiSource(QObject *loggerGui) {
  qDebug() << "logger gui element updated" << endl;
  connect(this, SIGNAL(totalPointChanged(QVariant)), loggerGui,
          SLOT(changeTotalPoint(QVariant)));
}

void LoggerModule::updateModule(int currBufIndex) {
  if (isLogging) {
    QMutexLocker enabledListLock(&enabledListMutex);
    QReadLocker newDataLock(newDataLock_[currBufIndex]);

    auto firstIndex = enabledIndexList.first();
    QList<QList<QPointF> *> &currBuffer = newDataBuffer[currBufIndex];

    for (auto pointIndex = 0; pointIndex < currBuffer[firstIndex]->size();
         ++pointIndex) {
      // record time value
      *(dataStream) << "\n" << currBuffer[firstIndex]->at(pointIndex).x();

      for (auto serieIndex = 0; serieIndex < enabledIndexList.size();
           ++serieIndex) {
        // record y value for each of the enabled serie
        *(dataStream)
            << "\t"
            << currBuffer[enabledIndexList[serieIndex]]->at(pointIndex).y();
        ++totalPoint; // TODO: may be make this more efficient
      }
    }
    emit totalPointChanged(QVariant(totalPoint));
  }

  //   qDebug() << "Emitted Logger done updating" << endl;
}
