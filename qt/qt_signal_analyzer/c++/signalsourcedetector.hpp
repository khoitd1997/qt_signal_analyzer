#pragma once

#include <string>

#include "channel_data.h"

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QStringList>

class SignalSourceDetector : public QObject {
  Q_OBJECT
  Q_PROPERTY(QStringList sources READ sources NOTIFY sourcesChanged)

 private:
  QStringList sources_;
  explicit SignalSourceDetector(QObject *parent = nullptr);

 public:
  QStringList      sources();
  Q_INVOKABLE void updateSources();

  static QObject *singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

 signals:
  void sourcesChanged();
};
