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

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "QEventLoop"
#include <QMutex>
#include <QQmlApplicationEngine>
#include <QReadWriteLock>
#include <QThread>
#include <QWaitCondition>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QXYSeries>
#include <QtCore/QObject>

#include <QDebug>

#include <QFile>
#include <QTextStream>

#include "dataworker.h"
#include "graphdatamodule.h"
#include "loggermodule.h"
#include "measuremodule.h"

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class DataSource : public QObject
{
  Q_OBJECT
  Q_PROPERTY(
    MeasureModule* measureModule READ measureModule NOTIFY measureModuleChanged)
  Q_PROPERTY(
    GraphDataModule* graphModule READ graphModule NOTIFY graphModuleChanged)
  Q_PROPERTY(
    LoggerModule* loggerModule READ loggerModule NOTIFY loggerModuleChanged)

signals:
  void startWork();
  void startUpdate();
  void startLoggerUpdate(int currBufferIndex_);
  void dataProcessDone();
  void measureModuleChanged();
  void graphModuleChanged();
  void loggerModuleChanged();

public:
  explicit DataSource(QObject* parent = nullptr);
  ~DataSource();

public slots:
  void start(void);
  void processData(void);
  void incrementFinished();
  MeasureModule* measureModule(void) const;
  GraphDataModule* graphModule(void) const;
  LoggerModule* loggerModule(void) const;

private:
  DataWorker* dataWorker_ = nullptr;
  MeasureModule* measureModule_ = nullptr;
  GraphDataModule* graphModule_ = nullptr;
  LoggerModule* loggerModule_ = nullptr;

  uint totalFinished_ = 0;
  //   QReadWriteLock dataLock;
  QMutex eventCounterLock;
  QEventLoop waitLoop;

  QThread dataWorkerThread_;
  QThread graphThread_;
  QThread measureThread_;
  QThread loggerThread_;

  static const int totalBuffer = 2;
  int currBufferIndex_ = 0;
  QList<QList<QPointF>*> allData_;
  QList<QList<QList<QPointF>*>> newDataBuffer_;
};

#endif // DATASOURCE_H
