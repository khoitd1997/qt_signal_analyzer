#include <QDebug>

#include "loggermodule.h"

LoggerModule::LoggerModule(QList<QList<QList<QPointF>*>>& newDataBuffer)
  : newDataBuffer(newDataBuffer)
{}

QString
LoggerModule::switchLogger(const bool isRecording,
                           const QVariant enabledList,
                           const QString destUrl,
                           const QVariant nameList)
{
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

void
LoggerModule::setGuiSource(QObject* measureGUI)
{
  qDebug() << "logger gui element updated" << endl;
  this->guiObj = measureGUI;
}

void
LoggerModule::updateLogger(int currBufferIndex)
{
  if (isLogging) {
    auto firstIndex = enabledIndexList.first();
    QList<QList<QPointF>*>& currBuffer = newDataBuffer[currBufferIndex];

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
    guiObj->setProperty("totalPoint", QVariant(totalPoint));
  }
  //  qDebug() << "Logger done updating" << endl;
  emit loggerFinished();
}
