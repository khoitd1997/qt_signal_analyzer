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
#include "graphdatamodule.h"
#include "loggermodule.h"
#include "mathmodule.h"
#include "measuremodule.h"

#include "signalsourcedetector.hpp"

#include <QApplication>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QtCore/QDir>
#include <QtDebug>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>

int main(int argc, char* argv[]) {
  // Qt Charts uses Qt Graphics View Framework for drawing, therefore
  // QApplication must be used.
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app(argc, argv);
  QQuickStyle::setStyle("Material");
  QQuickStyle::setFallbackStyle("Material");

  qmlRegisterType<SignalSourceDetector>("io.qt.devicedetector", 1, 0, "SignalSourceDetector");

  QQmlApplicationEngine engine;
  DataSource            dataSource(&engine);

  QThread         graphThread;
  GraphDataModule graphModule(dataSource.allData_, &dataSource.allDataLock_);
  dataSource.prepNewModule(&graphModule, &graphThread, false);
  qmlRegisterSingletonType<GraphDataModule>(
      "Qt.analyzer.graphModule", 1, 0, "GraphModule", GraphDataModule::singletonProvider);

  QThread    mathThread;
  MathModule mathModule(dataSource.allData_, &dataSource.allDataLock_);
  dataSource.prepNewModule(&mathModule, &mathThread, false);
  qmlRegisterSingletonType<MathModule>(
      "Qt.analyzer.mathModule", 1, 0, "MathModule", MathModule::singletonProvider);

  QThread      loggerThread;
  LoggerModule loggerModule(dataSource.newDataBuffer_, dataSource.newDataLock_);
  dataSource.prepNewModule(&loggerModule, &loggerThread, true);
  qmlRegisterSingletonType<LoggerModule>(
      "Qt.analyzer.loggerModule", 1, 0, "LoggerModule", LoggerModule::singletonProvider);

  QThread       measureThread;
  MeasureModule measureModule(dataSource.allData_, &dataSource.allDataLock_);
  dataSource.prepNewModule(&measureModule, &measureThread, false);
  qmlRegisterSingletonType<MeasureModule>(
      "Qt.analyzer.measureModule", 1, 0, "MeasureModule", MeasureModule::singletonProvider);

  engine.rootContext()->setContextProperty("dataSource", &dataSource);

  engine.load(QUrl("qrc:///qml/main.qml"));
  return app.exec();
}
