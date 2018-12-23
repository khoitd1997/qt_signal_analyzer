#ifndef MATHMODULE_H
#define MATHMODULE_H

#include "QList"
#include "QObject"
#include "QQmlListProperty"
#include "measureobj.h"

using MeasType = MeasureObj::MeasureType;

class Measuremodule : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QQmlListProperty<MeasureObj> measureObjs READ measureObjs NOTIFY
               measureObjsChanged)

public:
  //  ~Measuremodule();
  //  Measuremodule(const Measuremodule& other);
  explicit Measuremodule(QObject* parent = nullptr);
  QQmlListProperty<MeasureObj> measureObjs();
  ~Measuremodule();

signals:
  void measureObjsChanged();

public slots:
  void update();

private:
  static void appendMeasureObj(QQmlListProperty<MeasureObj>* list,
                               MeasureObj* measObj);
  static int countMeasureObj(QQmlListProperty<MeasureObj>*);
  static MeasureObj* atMeasureObj(QQmlListProperty<MeasureObj>*, int index);
  static void clearMeasureObj(QQmlListProperty<MeasureObj>* list);

  QList<MeasureObj*> m_measureObjs;
};

#endif // MATHMODULE_H
