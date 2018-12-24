#include "measuremodule.h"
#include "measureobj.h"

#include "QObject"
#include "QTimer"
#include "QtDebug"
#include <QQmlListProperty>
#include <QQmlProperty>
// Q_DECLARE_METATYPE(MeasureModule)
// Q_DECLARE_METATYPE(MeasureModule*)

MeasureModule::MeasureModule(QList<QList<QPointF>*>& dataList)
  : QObject()
  , m_data(dataList)
{
  measResult.append(4.1);
  measResult.append(5.1);
  measResult.append(6.1);
  measResult.append(7.1);
}

MeasureModule::~MeasureModule() {}

void
MeasureModule::setGuiSource(QObject* measureGUI)
{
  this->measureGUI = measureGUI;
}

QList<qreal>
MeasureModule::getResult(void)
{
  qDebug() << measResult[0] << endl;
  return measResult;
}

void
MeasureModule::updateMeasure()
{
  static bool isFirst = true;
  static qreal multiplier = 1;

  for (auto i = 0; i < measResult.size(); ++i) {
    measResult[i] = measResult[i] * multiplier;
  }

  isFirst = !isFirst;
  multiplier++;

  auto variant = (QQmlProperty::read(measureGUI, "typeIndex")).toList();
  qDebug() << "Type Index result: " << endl;
  for (auto i = 0; i < variant.size(); ++i) {
    qDebug() << i << variant[i].toInt() << endl;
  }

  variant = (QQmlProperty::read(measureGUI, "sourceList")).toList();
  qDebug() << "Source List result: " << endl;
  for (auto i = 0; i < variant.size(); ++i) {
    qDebug() << i << variant[i].toInt() << endl;
  }

  variant = (QQmlProperty::read(measureGUI, "enabledList")).toList();
  qDebug() << "Enabled List result: " << endl;
  for (auto i = 0; i < variant.size(); ++i) {
    qDebug() << i << variant[i].toBool() << endl;
  }

  emit measureFinished();
}
