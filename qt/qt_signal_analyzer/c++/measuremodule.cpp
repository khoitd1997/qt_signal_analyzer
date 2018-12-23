#include "measuremodule.h"
#include "measureobj.h"

#include "QObject"
#include "QTimer"
#include "QtDebug"
#include <QQmlListProperty>

using MeasType = MeasureObj::MeasureType;

Measuremodule::Measuremodule(QObject* parent)
  : QObject(parent)
{
  this->m_measureObjs.append(new MeasureObj(false, 15, MeasType::Freq, 0));
  this->m_measureObjs.append(new MeasureObj(false, 16, MeasType::Freq, 0));
  this->m_measureObjs.append(new MeasureObj(false, 17, MeasType::Freq, 0));
  this->m_measureObjs.append(new MeasureObj(false, 18, MeasType::Freq, 0));

  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(300);
}

Measuremodule::~Measuremodule()
{
  for (auto i = 0; i < this->m_measureObjs.size(); ++i) {
    delete this->m_measureObjs[i];
  }
}

QQmlListProperty<MeasureObj>
Measuremodule::measureObjs()
{
  return QQmlListProperty<MeasureObj>(this,
                                      this,
                                      &Measuremodule::appendMeasureObj,
                                      &Measuremodule::countMeasureObj,
                                      &Measuremodule::atMeasureObj,
                                      &Measuremodule::clearMeasureObj);
}

void
Measuremodule::appendMeasureObj(QQmlListProperty<MeasureObj>* list,
                                MeasureObj* measureObj)
{
  Measuremodule* measuremodule = qobject_cast<Measuremodule*>(list->object);
  if (measureObj) {
    measuremodule->m_measureObjs.append(measureObj);
  }
}

int
Measuremodule::countMeasureObj(QQmlListProperty<MeasureObj>* list)
{
  Measuremodule* measuremodule = qobject_cast<Measuremodule*>(list->object);
  return measuremodule->m_measureObjs.size();
}

MeasureObj*
Measuremodule::atMeasureObj(QQmlListProperty<MeasureObj>* list, int index)
{
  Measuremodule* measuremodule = qobject_cast<Measuremodule*>(list->object);
  return measuremodule->m_measureObjs[index];
}

void
Measuremodule::clearMeasureObj(QQmlListProperty<MeasureObj>* list)
{
  Measuremodule* measuremodule = qobject_cast<Measuremodule*>(list->object);

  for (auto i = 0; i < measuremodule->m_measureObjs.size(); ++i) {
    delete measuremodule->m_measureObjs[i];
  }
  measuremodule->m_measureObjs.clear();
}

void
Measuremodule::update()
{
  qDebug() << "Result is for 0 is:" << this->m_measureObjs[0]->isEnabled()
           << this->m_measureObjs[0]->value() << this->m_measureObjs[0]->type()
           << this->m_measureObjs[0]->targetIndex() << endl;

  this->m_measureObjs[0]->setValue(1 + (this->m_measureObjs[0]->value()));
  this->m_measureObjs[1]->setValue(1 + (this->m_measureObjs[1]->value()));
  this->m_measureObjs[2]->setValue(1 + (this->m_measureObjs[2]->value()));
  this->m_measureObjs[3]->setValue(1 + (this->m_measureObjs[3]->value()));
}
