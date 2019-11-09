#pragma once

#include <string>

#include "channel_data.h"

#include <QObject>
#include <QString>
#include <QStringList>

class SignalSourceDetector : public QObject {
  Q_OBJECT
  Q_PROPERTY(QStringList sources READ sources NOTIFY sourcesChanged)

 private:
  QStringList sources_;

 public:
  explicit SignalSourceDetector(QObject* parent = nullptr);

  QStringList      sources();
  Q_INVOKABLE void updateSources();

 signals:
  void sourcesChanged();
};
