#ifndef SCOPEMODULE_H
#define SCOPEMODULE_H

#include "QList"
#include <QObject>
#include <QPointF>
#include <QThread>

class ScopeModule : public QObject {
  Q_OBJECT
public:
  ScopeModule() = delete;
  explicit ScopeModule(QList<QList<QList<QPointF> *>> &newDataBuffer);
  virtual ~ScopeModule();

signals:
  void moduleFinished();

public slots:
  virtual void updateModule(const int currBufIndex) = 0;
  virtual void setGuiSource(QObject *guiObj);

protected:
  QList<QList<QList<QPointF> *>> &newDataBuffer_;
  QObject *guiObj_ = nullptr;
};

#endif // SCOPEMODULE_H
