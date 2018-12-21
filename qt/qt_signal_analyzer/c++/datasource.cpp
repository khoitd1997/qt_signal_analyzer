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
#include <QtCharts/QAreaSeries>
#include <QtCharts/QXYSeries>
#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>
#include <QtQuick/QQuickItem>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries*)
Q_DECLARE_METATYPE(QAbstractAxis*)
static const int maxPoints = 100000;
static const double storageThreshold = 0.8;

DataSource::DataSource(QQmlApplicationEngine* appEngine, QObject* parent)
  : QObject(parent)
  , m_appEngine(appEngine)
{
  qRegisterMetaType<QAbstractSeries*>();
  qRegisterMetaType<QAbstractAxis*>();

  m_data.reserve(maxPoints);
  generateData();
}

void
DataSource::update(QAbstractSeries* series)
{
  this->generateData();
  if (series) {
    QXYSeries* xySeries = static_cast<QXYSeries*>(series);
    // Use replace instead of clear + append, it's optimized for performance
    xySeries->replace(m_data);
    //    qDebug() << "Series Updated " << m_data[0].x() << m_data[0].y() <<
    //    endl;
  }
}

void
DataSource::generateData(void)
{
  static unsigned long currLimit = 0;
  if (m_data.size() > storageThreshold * maxPoints) {
    qDebug() << "Free elements" << endl;
    m_data.erase(m_data.begin(), m_data.begin() + m_data.size() / 2);
  }

  for (auto j(currLimit); j < currLimit + 1024; j++) {
    qreal x(0);
    qreal y(0);
    y =
      qSin(M_PI / 50 * j) + 0.5 + QRandomGenerator::global()->generateDouble();
    x = j;
    m_data.append(QPointF(x, y));
    //    qDebug() << "Generating x: " << x << endl;
  }
  currLimit += 1024;
}
