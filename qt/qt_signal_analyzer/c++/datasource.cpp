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

static const auto totalSerie = 4;
static const auto totalModule = 2;

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries*)
Q_DECLARE_METATYPE(QAbstractAxis*)

DataSource::DataSource(QObject* parent)
  : QObject(parent)
  , m_measureModule(new MeasureModule(this->m_data))
  , m_graphModule(new GraphDataModule(this->m_data))
{
  qRegisterMetaType<QAbstractSeries*>();
  qRegisterMetaType<QAbstractAxis*>();

  for (auto i = 0; i < totalSerie; ++i) {
    m_data.append(new QList<QPointF>());
    m_data[i]->reserve(maxPoints);
  }

  auto dataWorker = new DataWorker(dataLock, m_data);
  dataWorker->moveToThread(&dataThread);

  connect(&dataThread, &QThread::finished, dataWorker, &QObject::deleteLater);
  connect(this, &DataSource::startWork, dataWorker, &DataWorker::startWork);
  connect(
    dataWorker, &DataWorker::newDataReady, this, &DataSource::processData);

  dataThread.start();

  m_graphModule->moveToThread(&graphDataThread);
  connect(this,
          &DataSource::startUpdate,
          m_graphModule,
          &GraphDataModule::updateLineSeries);
  connect(m_graphModule,
          &GraphDataModule::graphDataFinished,
          this,
          &DataSource::incrementFinished);
  graphDataThread.start();

  m_measureModule->moveToThread(&measureDataThread);
  connect(this,
          &DataSource::startUpdate,
          m_measureModule,
          &MeasureModule::updateMeasure);
  connect(m_measureModule,
          &MeasureModule::measureFinished,
          this,
          &DataSource::incrementFinished);
  measureDataThread.start();
}

DataSource::~DataSource()
{
  dataThread.quit();
  dataThread.wait();
}

void
DataSource::incrementFinished()
{
  QMutexLocker locker(&eventCounterLock);
  ++totalFinished;
  if (totalFinished == totalModule) {
    totalFinished = 0;
    qDebug() << "All module done" << endl;
    waitLoop.quit();
  }
}

void
DataSource::start(void)
{
  emit startWork();
}

void
DataSource::processData(void)
{
  QReadLocker locker(&dataLock);
  qDebug() << "Begin data processing" << endl;
  emit startUpdate();

  waitLoop.exec(QEventLoop::ExcludeUserInputEvents |
                QEventLoop::ExcludeSocketNotifiers);

  qDebug() << "Event loop done" << endl;
  emit dataProcessDone();
}

MeasureModule*
DataSource::measureModule(void) const
{
  return m_measureModule;
}

GraphDataModule*
DataSource::graphModule(void) const
{
  return m_graphModule;
}
