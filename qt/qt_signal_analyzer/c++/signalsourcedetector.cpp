#include "signalsourcedetector.hpp"

#include <QDebug>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <memory>

SignalSourceDetector::SignalSourceDetector(QObject* parent) : QObject(parent) {}

QStringList SignalSourceDetector::sources() { return sources_; }

void SignalSourceDetector::updateSources() {
  QStringList temp;
  temp.append("Simulation");

  std::string serialDir{"/dev/serial/by-id"};
  if (std::filesystem::exists(serialDir)) {
    for (auto& p : std::filesystem::directory_iterator(serialDir)) {
      temp.append((std::filesystem::read_symlink(p.path()).filename()).c_str());
      qCritical() << temp.back();
    }
  }

  auto isChanged = false;
  if (temp.size() != sources_.size()) {
    isChanged = true;
  } else {
    temp.sort();
    for (auto i = 0; i < temp.size(); ++i) {
      if (temp.at(i) != sources_.at(i)) {
        isChanged = true;
        break;
      }
    }
  }

  if (isChanged) {
    sources_ = temp;
    emit sourcesChanged();
  }
}
