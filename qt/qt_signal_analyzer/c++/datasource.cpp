/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "datasource.h"
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QXYSeries>
#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>
#include <QtQuick/QQuickItem>

#include "dataworker.h"
#include "graphdatamodule.h"

static const auto TOTAL_SERIES = 4;
static const auto TOTAL_MODULE = 3;

static const int MAX_TOTAL_POINTS = 100000;
static const double STORAGE_THRESHOLD = 0.8;

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(QObject *parent)
    : QObject(parent), measureModule_(new MeasureModule(allData_)),
      graphModule_(new GraphDataModule(allData_)),
      loggerModule_(new LoggerModule(newDataBuffer_))

{
  qRegisterMetaType<QAbstractSeries *>();
  qRegisterMetaType<QAbstractAxis *>();

  for (auto i = 0; i < TOTAL_SERIES; ++i) {
    allData_.append(new QList<QPointF>());
    allData_[i]->reserve(MAX_TOTAL_POINTS);
  }

  for (auto i = 0; i < totalBuffer; ++i) {
    newDataBuffer_.append(QList<QList<QPointF> *>());
    for (auto j = 0; j < TOTAL_SERIES; ++j) {
      newDataBuffer_[i].append(new QList<QPointF>());
      newDataBuffer_[i][j]->reserve(NEW_POINTS_PER_EVENT);
    }
  }

  dataWorker_ = new DataWorker(newDataBuffer_);
  dataWorker_->moveToThread(&dataWorkerThread_);

  connect(&dataWorkerThread_, &QThread::finished, dataWorker_,
          &QObject::deleteLater);
  connect(this, &DataSource::startWork, dataWorker_, &DataWorker::startWork);
  connect(dataWorker_, &DataWorker::newDataReady, this,
          &DataSource::processData);
  dataWorkerThread_.start();

  graphModule_->moveToThread(&graphThread_);
  connect(&graphThread_, &QThread::finished, graphModule_,
          &QObject::deleteLater);
  connect(this, &DataSource::startUpdate, graphModule_,
          &GraphDataModule::updateLineSeries);
  connect(graphModule_, &GraphDataModule::graphDataFinished, this,
          &DataSource::incrementFinished);
  graphThread_.start();

  measureModule_->moveToThread(&measureThread_);
  connect(&measureThread_, &QThread::finished, measureModule_,
          &QObject::deleteLater);
  connect(this, &DataSource::startUpdate, measureModule_,
          &MeasureModule::updateMeasure);
  connect(measureModule_, &MeasureModule::measureFinished, this,
          &DataSource::incrementFinished);
  measureThread_.start();

  loggerModule_->moveToThread(&loggerThread_);
  connect(&loggerThread_, &QThread::finished, loggerModule_,
          &QObject::deleteLater);
  connect(this, &DataSource::startLoggerUpdate, loggerModule_,
          &LoggerModule::updateLogger);
  connect(loggerModule_, &LoggerModule::loggerFinished, this,
          &DataSource::incrementFinished);

  loggerThread_.start();
}

DataSource::~DataSource() {
  // TODO: add deallocation of resources
  dataWorkerThread_.quit();
  dataWorkerThread_.wait();
}

void DataSource::incrementFinished() {
  //   QMutexLocker locker(&eventCounterLock);
  ++totalFinished_;
  if (totalFinished_ == TOTAL_MODULE) {
    totalFinished_ = 0;
    // qDebug() << "All module done" << endl;
    waitLoop.quit();
  }
}

void DataSource::start(void) { emit startWork(); }

void DataSource::processData(void) {
  // move fresh data to the list of all data and trim the list if necessary
  for (auto i = 0; i < TOTAL_SERIES; ++i) {
    *(allData_[i]) << *(newDataBuffer_[currBufferIndex_][i]);
    if ((allData_[i])->size() > STORAGE_THRESHOLD * MAX_TOTAL_POINTS) {
      //   qDebug() << "Free elements" << endl;
      (allData_[i])
          ->erase((allData_[i])->begin(),
                  (allData_[i])->begin() + (allData_[i])->size() / 2);
    }
  }

  // remove data points to prevent overfilling

  //   QReadLocker locker(&dataLock);
  //   qDebug() << "Begin data processing" << endl;
  emit startUpdate();
  emit startLoggerUpdate(currBufferIndex_);

  waitLoop.exec(QEventLoop::ExcludeUserInputEvents |
                QEventLoop::ExcludeSocketNotifiers);

  //   qDebug() << "Event loop done" << endl;
  emit dataProcessDone();

  for (auto i = 0; i < TOTAL_SERIES; ++i) {
    newDataBuffer_[currBufferIndex_][i]->clear();
  }
  currBufferIndex_ =
      (currBufferIndex_ == totalBuffer - 1) ? 0 : currBufferIndex_ + 1;
}

MeasureModule *DataSource::measureModule(void) const { return measureModule_; }

GraphDataModule *DataSource::graphModule(void) const { return graphModule_; }

LoggerModule *DataSource::loggerModule(void) const { return loggerModule_; }
