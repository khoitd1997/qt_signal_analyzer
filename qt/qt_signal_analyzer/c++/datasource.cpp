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
#include "scopeconstants.h"

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(QObject *parent) : QObject(parent) {
  qRegisterMetaType<QAbstractSeries *>();
  qRegisterMetaType<QAbstractAxis *>();

  for (auto i = 0; i < kTotalSeries; ++i) {
    allData_.append(new QVector<QPointF>());
    allData_[i]->reserve(kMaxTotalPoints);
  }

  for (auto i = 0; i < kTotalNewDataBuffer; ++i) {
    newDataBuffer_.append(QList<QVector<QPointF> *>());
    newDataLock_.append(new QReadWriteLock());
    for (auto j = 0; j < kTotalSeries; ++j) {
      newDataBuffer_[i].append(new QVector<QPointF>());
      newDataBuffer_[i][j]->reserve(kNewPointsPerTransfer);
      for (auto k = 0; k < kNewPointsPerTransfer; ++k) {
        newDataBuffer_[i][j]->append(QPointF(5, 5));
      }
    }
  }

  dataWorker_ = new DataWorker(newDataBuffer_, newDataLock_);
  dataWorker_->moveToThread(&dataWorkerThread_);

  connect(&dataWorkerThread_, &QThread::finished, dataWorker_,
          &QObject::deleteLater);
  connect(this, &DataSource::startWork, dataWorker_, &DataWorker::startWork);
  connect(dataWorker_, &DataWorker::newDataReady, this,
          &DataSource::processData);
  dataWorkerThread_.start();
}

void DataSource::prepNewModule(QObject *scopeModule, QThread *moduleThread,
                               const bool needNewData) {
  scopeModule->moveToThread(moduleThread);
  connect(moduleThread, &QThread::finished, scopeModule, &QObject::deleteLater);
  if (needNewData) {
    connect(this, SIGNAL(startUpdateWithNewData(int)), scopeModule,
            SLOT(updateModule(int)));
  } else {
    connect(this, SIGNAL(startUpdate()), scopeModule, SLOT(updateModule()));
  }
  moduleThread->start();
}

DataSource::~DataSource() {
  // TODO: add deallocation of resources
  dataWorkerThread_.quit();
  dataWorkerThread_.wait();
}

void DataSource::start(void) { emit startWork(); }

void DataSource::processData(const int curBufIndex) {
  // move fresh data to the list of all data and trim the list if necessary
  {
    QWriteLocker lock(&allDataLock_);
    QReadLocker readLock(newDataLock_[curBufIndex]);
    for (auto i = 0; i < kTotalSeries; ++i) {
      (allData_[i])->append(*(newDataBuffer_[curBufIndex][i]));

      //       remove data points to prevent overfilling
      if ((allData_[i])->size() > kStorageThreshold * kMaxTotalPoints) {
        //   qDebug() << "Free elements" << endl;
        (allData_[i])
            ->erase((allData_[i])->begin(),
                    (allData_[i])->begin() + (allData_[i])->size() / 2);
      }
    }
  }

  emit startUpdate();
  emit startUpdateWithNewData(curBufIndex);
}
